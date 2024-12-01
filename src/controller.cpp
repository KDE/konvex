// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "controller.h"

#include "konvexplugininterface.h"

#include <KPluginFactory>

#ifdef HAVE_WINDOWSYSTEM
#include <KWindowEffects>
#include <QQuickWindow>
#endif

Controller::Controller(QObject *parent)
    : QObject{parent}
{
}

void Controller::setBlur(QQuickItem *item, bool blur)
{
#ifdef HAVE_WINDOWSYSTEM
    auto setWindows = [item, blur]() {
        auto reg = QRect(QPoint(0, 0), item->window()->size());
        KWindowEffects::enableBackgroundContrast(item->window(), blur, 1, 1, 1, reg);
        KWindowEffects::enableBlurBehind(item->window(), blur, reg);
    };

    disconnect(item->window(), &QQuickWindow::heightChanged, this, nullptr);
    disconnect(item->window(), &QQuickWindow::widthChanged, this, nullptr);
    connect(item->window(), &QQuickWindow::heightChanged, this, setWindows);
    connect(item->window(), &QQuickWindow::widthChanged, this, setWindows);
    setWindows();
#endif
}

QQuick3DGeometry *Controller::createGeometry(const QUrl &url, QQuick3DObject *parent)
{
    if (url.isEmpty()) {
        return nullptr;
    }

    auto plugins = KPluginMetaData::findPlugins(QStringLiteral("kf6/konvex"), [](const KPluginMetaData &) {
        return true;
    });
    if (plugins.empty()) {
        qWarning() << "No plugins found.";
        return nullptr;
    }

    if (auto result = KPluginFactory::instantiatePlugin<KonvexPluginInterface>(plugins.first(), this, QVariantList())) {
        return result.plugin->createGeometry(url.toLocalFile(), parent);
    } else {
        qWarning() << result.errorString;
        return nullptr;
    }
}
