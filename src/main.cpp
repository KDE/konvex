// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KAboutData>
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

    KAboutData about(QStringLiteral("konvex"),
                     i18n("Konvex"),
                     QStringLiteral("1.0.0"),
                     i18n("3D Model Viewer"),
                     KAboutLicense::GPL_V3,
                     i18n("© 2023 Joshua Goins"));
    about.addAuthor(i18n("Joshua Goins"), i18n("Maintainer"), QStringLiteral("josh@redstrate.com"), QStringLiteral("https://redstrate.com/"));
    about.setOrganizationDomain("kde.org");

    KAboutData::setApplicationData(about);

    QGuiApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("org.kde.konvex")));

    QQmlApplicationEngine engine;

    auto controller = new Controller();

    qmlRegisterSingletonInstance("org.kde.kmodelviewer", 1, 0, "Controller", controller);
    qmlRegisterSingletonType("org.kde.kmodelviewer", 1, 0, "About", [](QQmlEngine *engine, QJSEngine *) -> QJSValue {
        return engine->toScriptValue(KAboutData::applicationData());
    });

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
        Q_EMIT controller->fileOpened("file:///" + parser.positionalArguments()[0]);
    }

    return app.exec();
}
