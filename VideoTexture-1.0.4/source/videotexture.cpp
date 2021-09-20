#include "videotexture.h"

#include <QDebug>
#include <QMutex>
#include <QMutexLocker>
#include <QString>
#include <QOpenGLContext>

/* Copied from Vivante EGL OES Extension Support Document */
#define GL_VIV_YV12     0x8FC0
#define GL_VIV_NV12     0x8FC1
#define GL_VIV_YUY2     0x8FC2
#define GL_VIV_UYVY     0x8FC3
#define GL_VIV_NV21     0x8FC4

VideoTexture::VideoTexture(QOpenGLContext *gl_context, const QString& memoryLocation, int memorySize, const QSize& size, const QString& format)
    : m_memory_lock(new QMutex),
      m_texture_id(0),
      m_size(size),
      m_memoryLocation(memoryLocation),
      m_memorySize(memorySize),
      m_format(format),
      m_handle(-1),
      m_memory(NULL),
      m_hw_memory(0),
      m_refresh_settings(true),
      m_glFormat(stringToFormat(m_format)),
      m_glType(stringToType(m_format))
{
    initializeOpenGLFunctions();
    glGenTextures = (glGenTextures_t) gl_context->getProcAddress("glGenTextures");
    glBindTexture = (glBindTexture_t) gl_context->getProcAddress("glBindTexture");
    glTexImage2D = (glTexImage2D_t) gl_context->getProcAddress("glTexImage2D");
    
#ifdef ENABLE_VIVANTE
    glTexDirectVIV = (glTexDirectVIV_t) gl_context->getProcAddress("glTexDirectVIV");
    glTexDirectVIVMap = (glTexDirectVIVMap_t) gl_context->getProcAddress("glTexDirectVIVMap");
    glTexDirectInvalidateVIV = (glTexDirectInvalidateVIV_t) gl_context->getProcAddress("glTexDirectInvalidateVIV");
#else
    glTexDirectVIV = NULL;
    glTexDirectVIVMap = NULL;
    glTexDirectInvalidateVIV = NULL;
#endif

    off_t offset = 0;

    if (m_memoryLocation.startsWith(":dma:")) {
        qDebug() << __FILE__ << ":" << __func__ << ": m_memoryLocation = " << m_memoryLocation ;
        m_handle = open("/dev/mxc_ipu", O_RDONLY);
        if (m_handle == -1) qCritical("Failed to open /dev/mxc_ipu: %s", strerror(errno));
        m_hw_memory = m_memoryLocation.section(':', 2, 2).toInt(nullptr, 16);
        offset = m_hw_memory;
    } else {
        m_handle = shm_open(qPrintable(m_memoryLocation), O_RDWR | O_CREAT, 0666);
        if (m_handle == -1) qCritical("failed to open shared memory %s: %s", qPrintable(m_memoryLocation), strerror(errno));
    }

    m_memory = mmap(NULL,
                    m_memorySize,
                    PROT_READ,
                    MAP_SHARED,
                    m_handle,
                    offset);
    if (m_memory == MAP_FAILED) {
        m_memory = nullptr;
        qCritical("failed to map shared memory: %s", strerror(errno));
    }
}

void VideoTexture::bind()
{
    QMutexLocker lock(m_memory_lock);
    if (m_size == QSize(0,0)) return;
    if (m_memory == NULL) return;
    
    glBindTexture(GL_TEXTURE_2D, textureId());
    
    if (m_hw_memory && glTexDirectVIVMap && isVivSupportedFormat(m_glFormat)) {
        if (m_refresh_settings) {
            m_refresh_settings = false;
            glTexDirectVIVMap(GL_TEXTURE_2D,
                              m_size.width(),
                              m_size.height(),
                              m_glFormat,
                              &m_memory,
                              (const GLuint*) &m_hw_memory);
        } else {
            glTexDirectInvalidateVIV(GL_TEXTURE_2D);
        }
    } else if (isTexSupportedFormat(m_glFormat)) {
        m_refresh_settings = false;        
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     m_glFormat,
                     m_size.width(),
                     m_size.height(),
                     0,
                     m_glFormat,
                     m_glType,
                     m_memory);
    } else {
        qDebug() << "Unsupported Video Format " << formatToString(m_glFormat);
        return;
    }
    
    updateBindOptions(true);
}

bool VideoTexture::updateTexture()
{
    return true;
}

QSize VideoTexture::textureSize() const
{
    return m_size;
}

int VideoTexture::textureId() const
{
    if (m_texture_id == 0) {
        glGenTextures(1, &const_cast<VideoTexture*>(this)->m_texture_id);
    }
    return m_texture_id;
}

bool VideoTexture::isVivSupportedFormat(GLenum format)
{
    switch (format) {
        case GL_VIV_YV12:
        case GL_VIV_NV12:
        case GL_VIV_YUY2:
        case GL_VIV_UYVY:
        case GL_VIV_NV21:
            return true;
        default:
            return false;
    }
}

bool VideoTexture::isTexSupportedFormat(GLenum format)
{
    switch (format) {
        case GL_RGB:
        case GL_RGBA:
            return true;
        default:
            return false;
    }
}

QString VideoTexture::formatToString(GLenum format)
{
    switch (format) {
        case GL_RGBA:
            return "RGBA";
        case GL_RGB:
            return "RGB";
        case GL_VIV_NV12:
            return "NV12";
        case GL_VIV_YV12:
            return "YV12";
        case GL_VIV_UYVY:
            return "UYVY";
        case GL_VIV_YUY2:
            return "YUY2";
        case GL_VIV_NV21:
            return "NV21";
        default:
            return "";
    }
}

GLenum VideoTexture::stringToFormat(QString format)
{
    if (format == "RGBA") {
        return GL_RGBA;
    } else if (format == "RGB") {
        return GL_RGB;
    } else if (format == "YV12") {
        return GL_VIV_YV12;
    } else if (format == "NV12") {
        return GL_VIV_NV12;
    } else if (format == "YUY2") {
        return GL_VIV_YUY2;
    } else if (format == "UYVY") {
        return GL_VIV_UYVY;
    } else if (format == "NV21") {
        return GL_VIV_NV21;
    }
    
    return 0;
}

GLenum VideoTexture::stringToType(QString format)
{
    if (format == "RGBA") {
        return GL_UNSIGNED_BYTE;
    } else if (format == "RGB16") {
        return GL_UNSIGNED_SHORT_5_6_5;
    }
    
    return 0;
}
