// SPDX-FileCopyrightText: 2024 Joshua Goins <josh@redstrate.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

#include "konvex_export.h"

class QQuick3DGeometry;
class QQuick3DObject;

class KONVEXINTERFACE_EXPORT KonvexPluginInterface : public QObject
{
    Q_OBJECT
public:
    KonvexPluginInterface(QObject *parent)
        : QObject(parent)
    {
    }

    virtual QQuick3DGeometry *createGeometry(const QString &url, QQuick3DObject *paren) = 0;
};
