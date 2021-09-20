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

if (ENABLE_VIVANTE)
    add_definitions(-DENABLE_VIVANTE)
endif()

include_directories(include ${QTGSTREAMER_INCLUDES})

file(GLOB VideoTexture_SOURCES include/*.h source/*.cpp)
add_library(VideoTexture SHARED ${VideoTexture_SOURCES})
target_link_libraries(VideoTexture Qt5::Core Qt5::Qml Qt5::Quick Qt5::OpenGL)
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

set(QML_PLUGIN_PATH "${_qt5Qml_install_prefix}" CACHE STRING "QML Plugin Path")
message("-- VideoTexture install directory ${QML_PLUGIN_PATH}/qml/VideoTexture")
install(TARGETS VideoTexture LIBRARY DESTINATION "${QML_PLUGIN_PATH}/qml/VideoTexture")
install(FILES ${CMAKE_SOURCE_DIR}/videotexture.qmldir DESTINATION "${QML_PLUGIN_PATH}/qml/VideoTexture" RENAME qmldir)

