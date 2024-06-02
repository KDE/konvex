// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QQuickItem>

class Controller : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit Controller(QObject *parent = nullptr);

    Q_INVOKABLE void setBlur(QQuickItem *item, bool blur);

Q_SIGNALS:
    void fileOpened(QString path);
};