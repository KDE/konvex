// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KAboutData>
#include <KLocalizedQmlContext>
#include <KLocalizedString>
#include <QCommandLineParser>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "controller.h"

int main(int argc, char *argv[])
{
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

    QCommandLineParser parser;
    parser.setApplicationDescription(i18nc("Application Description", "3D Model Viewer"));
    parser.addPositionalArgument(QStringLiteral("urls"), i18nc("@info:shell", "Model file"));

    parser.process(app);

    KLocalization::setupLocalizedContext(&engine);
    KLocalizedString::setApplicationDomain("konvex");

    engine.loadFromModule(QStringLiteral("org.kde.konvex"), QStringLiteral("Main"));

    if (parser.positionalArguments().length() > 0) {
        auto controller = engine.singletonInstance<Controller *>(QStringLiteral("org.kde.konvex"), QStringLiteral("Controller"));
        Q_EMIT controller->fileOpened(QStringLiteral("file:///%1").arg(parser.positionalArguments()[0]));
    }

    return app.exec();
}
