#include "player.h"

#include <QGuiApplication>
#include <QQuickView>

#include <QTimer>

#include <QGst/Init>

int main(int argc, char** argv)
{
    QGst::init(&argc, &argv);
    QGuiApplication qapp(argc, argv);
    
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile("main.qml"));
    QObject *root = (QObject*) view.rootObject();
    
    if (root == NULL) {
        qDebug() << "unable to find videoTexture child";
    }
    
    Player player0(root->findChild<QObject*>("videoTexture0"), 0);
    Player player1(root->findChild<QObject*>("videoTexture1"), 1);
    Player player2(root->findChild<QObject*>("videoTexture2"), 2);
    Player player3(root->findChild<QObject*>("videoTexture3"), 3);
        
    QTimer* timer = new QTimer(&view);
    QObject::connect(timer, SIGNAL(timeout()), &view, SLOT(update()));
    timer->start(30);
    
    view.show();
    return qapp.exec();
}