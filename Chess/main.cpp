#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "gamemanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<GameManager>("chess.logics", 1, 0, "GameManager");
    GameManager manager;

    QQmlContext *pQmlcontext = engine.rootContext();
    if (!pQmlcontext) {
        qWarning() << "Cannot get QML context";
        return 0;
    }

    pQmlcontext->setContextProperty("gameController", &manager);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
