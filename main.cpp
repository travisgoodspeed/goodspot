#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include "qmlapplicationviewer.h"
#include "spotconnect.h"

//SpotConnect spotConnect;
SpotConnect *sc;

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication *app=new QApplication(argc,argv);
    QmlApplicationViewer *viewer=new QmlApplicationViewer();
    sc=new SpotConnect();
    viewer->rootContext()->setContextProperty("SpotConnect",sc);

    //viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);

    viewer->setMainQmlFile(QLatin1String("qml/goodspot/main.qml"));


    viewer->showExpanded();



    return app->exec();
}
