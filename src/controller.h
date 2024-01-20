// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QQuickItem>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

    Q_INVOKABLE void setBlur(QQuickItem *item, bool blur);

Q_SIGNALS:
    void fileOpened(QString path);
};

#endif // CONTROLLER_H
