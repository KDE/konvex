// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Window
import org.kde.kirigami as Kirigami
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigamiaddons.formcard as FormCard
import org.kde.konvex

FormCard.FormCardPage {
    id: page

    title: i18nc("@title:window", "Settings")

    FormCard.FormCard {
        Layout.topMargin: Kirigami.Units.largeSpacing
        Layout.fillWidth: true

        Component {
            id: aboutPage
            FormCard.AboutPage {
            }
        }
        Component {
            id: aboutKDEPage
            FormCard.AboutKDE {
            }
        }

        FormCard.FormButtonDelegate {
            text: i18nc("@action:button", "About Konvex")
            icon.name: "help-about-symbolic"
            onClicked: applicationWindow().pageStack.layers.push(aboutPage)
        }

        FormCard.FormDelegateSeparator {
        }

        FormCard.FormButtonDelegate {
            text: i18nc("@action:button", "About KDE")
            icon.name: "kde"
            onClicked: applicationWindow().pageStack.layers.push(aboutKDEPage)
        }
    }
}
