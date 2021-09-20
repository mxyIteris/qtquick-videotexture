cmake_minimum_required(VERSION 2.8.11)

project(VideoTexture)

option(ENABLE_VIVANTE "Enable Vivante OpenGL Extensions" ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-std=c++11)
endif()

find_package(Qt5Core)
find_package(Qt5Qml)
find_package(Qt5Quick)
find_package(Qt5OpenGL) 
find_package(Qt5DBus)

find_path(QTGSTREAMER_INCLUDES QGst/Init HINTS ${CMAKE_FIND_ROOT_PATH} PATH_SUFFIXES Qt5GStreamer)
find_library(Qt5GStreamer_LIBRARY NAMES Qt5GStreamer-0.10 HINTS ${CMAKE_FIND_ROOT_PATH})
find_library(Qt5GStreamerUtils_LIBRARY NAMES Qt5GStreamerUtils-0.10 HINTS ${CMAKE_FIND_ROOT_PATH})
find_library(Qt5GLib_LIBRARY NAMES Qt5GLib-2.0 HINTS ${CMAKE_FIND_ROOT_PATH})

find_library(RT_LIBRARY NAMES rt librt HINTS ${CMAKE_FIND_ROOT_PATH})

set(QTGSTREAMER_LIBRARIES ${Qt5GStreamer_LIBRARY} ${Qt5GStreamerUtils_LIBRARY} ${Qt5GLib_LIBRARY} ${RT_LIBRARY})

if (ENABLE_VIVANTE)
    add_definitions(-DENABLE_VIVANTE)
endif()

include_directories(include ${QTGSTREAMER_INCLUDES})

file(GLOB VideoTexture_SOURCES include/*.h source/*.cpp)
add_library(VideoTexture SHARED ${VideoTexture_SOURCES})
target_link_libraries(VideoTexture Qt5::Core Qt5::Qml Qt5::Quick Qt5::OpenGL ${QTGSTREAMER_LIBRARIES})
add_custom_command(TARGET VideoTexture POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/$<CONFIGURATION>/VideoTexture)
add_custom_command(TARGET VideoTexture POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   $<TARGET_FILE:VideoTexture> ${CMAKE_BINARY_DIR}/$<CONFIGURATION>/VideoTexture/
                   MAIN_DEPENDENCY $<TARGET_FILE:VideoTexture>)
add_custom_command(TARGET VideoTexture POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   ${CMAKE_SOURCE_DIR}/videotexture.qmldir ${CMAKE_BINARY_DIR}/$<CONFIGURATION>/VideoTexture/qmldir
                   MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/videotexture.qmldir)

file(GLOB Example_SOURCES example/*.h example/*.cpp)
add_executable(VideoTextureExample ${Example_SOURCES} example/main.qml)
target_link_libraries(VideoTextureExample VideoTexture ${QTGSTREAMER_LIBRARIES})
add_custom_command(TARGET VideoTextureExample POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   ${CMAKE_SOURCE_DIR}/example/main.qml ${CMAKE_BINARY_DIR}/$<CONFIGURATION>/
                   MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/example/main.qml)
                   
set(QML_PLUGIN_PATH "${_qt5Qml_install_prefix}" CACHE STRING "QML Plugin Path")
message("-- VideoTexture install directory ${QML_PLUGIN_PATH}/qml/VideoTexture")
install(TARGETS VideoTexture LIBRARY DESTINATION "${QML_PLUGIN_PATH}/qml/VideoTexture")
install(FILES ${CMAKE_SOURCE_DIR}/videotexture.qmldir DESTINATION "${QML_PLUGIN_PATH}/qml/VideoTexture" RENAME qmldir)

