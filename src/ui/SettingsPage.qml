// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Window 2.15
import org.kde.kirigami 2.20 as Kirigami
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigamiaddons.formcard as FormCard
import org.kde.kmodelviewer 1.0

FormCard.FormCardPage {
    id: page

    title: i18nc("@title:window", "Settings")

    FormCard.FormCard {
        Layout.topMargin: Kirigami.Units.largeSpacing
        Layout.fillWidth: true

        Component {
            id: aboutPage
            FormCard.AboutPage {
                aboutData: About
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
