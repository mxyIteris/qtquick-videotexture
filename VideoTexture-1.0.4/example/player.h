#ifndef __player_h__
#define __player_h__

#include <QObject>
#include <QSize>
#include <QString>
#include <QGst/Pipeline>

class Player : public QObject
{
    Q_OBJECT
    
public:
    explicit Player(QObject* videoTexture, int videoId, QObject* parent = NULL);
    
signals:
    void sizeChanged(QSize size);
    void formatChanged(QString format);
    void memoryChanged(QString memory, QString hw_memory);
    
public slots:
    void play();
    void pause();
    
protected:
    void capsUpdated();
    void memoryUpdated();
    
private:
    QGst::PipelinePtr m_pipeline;
    QGst::ElementPtr m_memsink;
};

#endif /* __player_h__ */