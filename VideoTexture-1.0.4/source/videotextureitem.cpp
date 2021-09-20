#include "videotextureitem.h"
#include "videotexture.h"

#include <QSGSimpleTextureNode>
#include <QQuickWindow>

#include <QDebug>

VideoTextureItem::VideoTextureItem(QQuickItem *parent) :
    QQuickItem(parent),
    m_texture(nullptr),
    m_node(nullptr),
    m_memorySize(0),
    m_size(QSize(0, 0)),
    m_ready(false)
{
    qDebug() << "Creating VideoTextureItem";
    /* This causes updatePaintNode to be called by the Scene Graph */
    setFlag(ItemHasContents, true);
}

VideoTextureItem::~VideoTextureItem()
{

}

QSGNode *VideoTextureItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData*)
{
    QSGSimpleTextureNode* node = static_cast<QSGSimpleTextureNode*>(oldNode);

    if (!node && m_ready) {
        if (node) delete node;
        m_node = node = new QSGSimpleTextureNode();
        qDebug() << "Creating VideoTexture " << m_memoryLocation << " (" << m_memorySize << " bytes) " << m_format << " " << m_size;
        m_texture = new VideoTexture(window()->openglContext(), m_memoryLocation, m_memorySize, m_size, m_format);
        node->setTexture(m_texture);
    }

    if (node) node->setRect(boundingRect());
    
    return node;
}

void VideoTextureItem::setMemoryLocation(const QString &memoryLocation)
{
    m_memoryLocation = memoryLocation;
}

void VideoTextureItem::setFormat(const QString &format)
{
    m_format = format;
}

void VideoTextureItem::setSize(const QSize &size)
{
    m_size = size;
}

void VideoTextureItem::setMemorySize(int memorySize)
{
    m_memorySize = memorySize;
}

void VideoTextureItem::setReady(bool ready)
{
    m_ready = ready;
}

void VideoTextureItem::textureResized(QSize)
{
    /* Silly hack to force the scene graph to refresh the resized texture. */
    if (m_node) {
        m_node->setTexture(m_texture);
        m_node->markDirty(QSGNode::DirtyMaterial | QSGNode::DirtyGeometry | QSGNode::DirtyForceUpdate);
    }
}
