// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KLocalizedContext>
#include <KLocalizedString>
#include <QCommandLineParser>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "controller.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    // workaround QTBUG-83871 where hiding the scene freezes the application
    qputenv("QT3D_SCENE3D_STOP_RENDER_HIDDEN", "1");

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }

    KLocalizedString::setApplicationDomain("konvex");

    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setApplicationName(QStringLiteral("konvex"));

    QGuiApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("org.kde.konvex")));

    QQmlApplicationEngine engine;

    auto controller = new Controller();

    qmlRegisterSingletonInstance("org.kde.kmodelviewer", 1, 0, "Controller", controller);

    QCommandLineParser parser;
    parser.setApplicationDescription(i18nc("Application Description", "3D Model Viewer"));
    parser.addPositionalArgument(QStringLiteral("urls"), i18nc("@info:shell", "Model file"));

    parser.process(app);

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    KLocalizedString::setApplicationDomain("konvex");

    const QUrl url(QStringLiteral("qrc:/ui/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    if (parser.positionalArguments().length() > 0) {
        Q_EMIT controller->fileOpened(parser.positionalArguments()[0]);
    }

    return app.exec();
}
