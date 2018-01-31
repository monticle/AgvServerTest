#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>

#include "msgcenter.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);


    //设置跟路径
    QString g_strExeRoot = QCoreApplication::applicationDirPath();
    QDir::setCurrent(g_strExeRoot);

    MsgCenter msgCenter;
    msgCenter.init();

    QQmlApplicationEngine engine;

    QQmlContext *ctxt = engine.rootContext();
    ctxt->setContextProperty("g_strExeRoot",g_strExeRoot);
    ctxt->setContextProperty("msgCenter",(QObject *)&msgCenter);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
