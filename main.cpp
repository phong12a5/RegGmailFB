#include <QCoreApplication>
//#include <QQmlApplicationEngine>
#include "AppMain.h"

int main(int argc, char *argv[])
{
    LOG << "STARTING ....";
    LOG << "CURRENT DIR: " << QDir::currentPath();

    srand(time(nullptr));

    QCoreApplication app(argc, argv);
//    QQmlApplicationEngine engine;

//    LOG << ADBCommand::screenShot("CB5A23BXPR");

    AppMain::instance()->initApplication();
    AppMain::instance()->startProgram();
//    engine.rootContext()->setContextProperty("AppMain",AppMain::instance());

//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    if (engine.rootObjects().isEmpty()){
//        LOG << "rootObject is NULL";
//    }

    return app.exec();
}



