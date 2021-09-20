#ifndef __videotexture_h__
#define __videotexture_h__

#include <qopengl.h>
#include <QSGDynamicTexture>
#include <QSize>
#include <QOpenGLFunctions>

#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>

class QMutex;
class QOpenGLContext;

typedef void (*glGenTextures_t)(GLsizei, GLuint*);
typedef void (*glBindTexture_t)(GLenum, GLuint);
typedef void (*glTexImage2D_t)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*);

typedef void (*glTexDirectVIV_t)(GLenum, GLsizei, GLsizei, GLenum, GLvoid**);
typedef void (*glTexDirectVIVMap_t)(GLenum, GLsizei, GLsizei, GLenum, GLvoid**, const GLuint*);
typedef void (*glTexDirectInvalidateVIV_t)(GLenum);


class VideoTexture : public QSGDynamicTexture, protected QOpenGLFunctions
{
    Q_OBJECT
    
public:
    explicit VideoTexture(QOpenGLContext *gl_context, const QString& memoryLocation, int memorySize, const QSize& size, const QString& format);

    void bind();
    bool updateTexture();

    QSize textureSize() const;
    int textureId() const;

    bool hasAlphaChannel() const { return false; }
    bool hasMipmaps() const { return false; }
    
signals:
    void textureResized(QSize size);
    
protected:
    glGenTextures_t             glGenTextures;
    glBindTexture_t             glBindTexture;
    glTexImage2D_t              glTexImage2D;
    
    /* Vivante DMA Support */
    glTexDirectVIV_t            glTexDirectVIV;
    glTexDirectVIVMap_t         glTexDirectVIVMap;
    glTexDirectInvalidateVIV_t  glTexDirectInvalidateVIV;

private:
    bool isVivSupportedFormat(GLenum format);
    bool isTexSupportedFormat(GLenum format);
    QString formatToString(GLenum format);
    GLenum stringToFormat(QString format);
    GLenum stringToType(QString format);
    
    QMutex  *m_memory_lock;
        
    GLuint  m_texture_id;
    QSize   m_size;

    QString m_memoryLocation;
    int     m_memorySize;
    QString m_format;

    int     m_handle;
    void*   m_memory;
    quint32 m_hw_memory;

    bool    m_refresh_settings;
    GLenum  m_glFormat;
    GLenum  m_glType;
};

#endif /* __videotexture_h__ */
