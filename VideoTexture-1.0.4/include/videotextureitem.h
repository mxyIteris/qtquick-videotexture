#ifndef __videotextureitem_h__
#define __videotextureitem_h__

#include <QQuickItem>

class VideoTexture;
class QSGSimpleTextureNode;

class VideoTextureItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString format READ format WRITE setFormat)
    Q_PROPERTY(QString memoryLocation READ memoryLocation() WRITE setMemoryLocation)
    Q_PROPERTY(QSize size READ size WRITE setSize)
    Q_PROPERTY(int memorySize READ memorySize() WRITE setMemorySize)
    Q_PROPERTY(bool ready READ ready WRITE setReady)
    
public:
    explicit VideoTextureItem(QQuickItem *parent = 0);
    ~VideoTextureItem();

    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *);
    
    const QString& format() const { return m_format; }
    const QString& memoryLocation() const { return m_memoryLocation; }
    const QSize& size() const { return m_size; }
    int memorySize() const { return m_memorySize; }
    bool ready() const { return m_ready; }

public slots:
    void setFormat(const QString& format);
    void setMemoryLocation(const QString& memoryLocation);
    void setMemorySize(int memorySize);
    void setSize(const QSize& size);
    void setReady(bool ready);
    
private slots:
    void textureResized(QSize size);

private:
    VideoTexture *m_texture;
    QSGSimpleTextureNode *m_node;
    QString m_format;
    QSize   m_size;
    QString m_memoryLocation;
    int     m_memorySize;
    bool    m_ready;
};

#endif /* __videotextureitem_h__ */
