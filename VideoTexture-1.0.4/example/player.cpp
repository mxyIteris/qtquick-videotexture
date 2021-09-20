#include "player.h"

#include <QString>
#include <QGst/Parse>
#include <QGlib/Connect>

#include <QDebug>

Player::Player(QObject *videoTexture, int videoId, QObject* parent)
    : QObject(parent)
{
    QString memsink_name;
    QString pipeline_arg;
    
    memsink_name = memsink_name.sprintf("memsink%d", videoId);
    pipeline_arg = pipeline_arg.sprintf("videotestsrc ! video/x-raw-rgb,width=352,height=288 ! memsink name=memsink%d", videoId);
    
    m_pipeline = QGst::Parse::launch(pipeline_arg).dynamicCast<QGst::Pipeline>();
    m_memsink = m_pipeline->getElementByName(memsink_name.toStdString().data());
    
    QGlib::connect(m_memsink, "caps-updated", this, &Player::capsUpdated);
    QGlib::connect(m_memsink, "memory-updated", this, &Player::memoryUpdated);
    
    connect(this, SIGNAL(formatChanged(QString)), videoTexture, SLOT(formatChanged(QString)));
    connect(this, SIGNAL(sizeChanged(QSize)), videoTexture, SLOT(sizeChanged(QSize)));
    connect(this, SIGNAL(memoryChanged(QString, QString)), videoTexture, SLOT(memoryChanged(QString, QString)));
    
    play();
}

void Player::play()
{
    m_pipeline->setState(QGst::StatePlaying);
}

void Player::pause()
{
    m_pipeline->setState(QGst::StatePaused);
}

void Player::capsUpdated()
{
    QString format = m_memsink->property("target-format").toString();
    QSize size(m_memsink->property("width").toInt(), m_memsink->property("height").toInt());

    emit formatChanged(format);
    emit sizeChanged(size);
}

void Player::memoryUpdated()
{
    QString memory = m_memsink->property("address").toString();
    QString hw_memory = m_memsink->property("physical-address").toString();
    
    emit memoryChanged(memory, hw_memory);
}
