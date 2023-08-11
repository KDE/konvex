// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Window 2.15
import org.kde.kirigami 2.20 as Kirigami
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigamiaddons.labs.mobileform 0.1 as MobileForm
import org.kde.kmodelviewer 1.0

Kirigami.ScrollablePage {
    id: page

    title: i18nc("@title:window", "Settings")

    ColumnLayout {
        width: parent.width

        MobileForm.FormCard {
            Layout.topMargin: Kirigami.Units.largeSpacing
            Layout.fillWidth: true
            contentItem: ColumnLayout {
                spacing: 0
                Component {
                    id: aboutPage
                    MobileForm.AboutPage {
                        aboutData: About
                    }
                }
                Component {
                    id: aboutKDEPage
                    MobileForm.AboutKDE {
                    }
                }

                MobileForm.FormButtonDelegate {
                    text: i18nc("@action:button", "About Konvex")
                    icon.name: "help-about-symbolic"
                    onClicked: applicationWindow().pageStack.layers.push(aboutPage)
                }

                MobileForm.FormDelegateSeparator {
                }

                MobileForm.FormButtonDelegate {
                    text: i18nc("@action:button", "About KDE")
                    icon.name: "kde"
                    onClicked: applicationWindow().pageStack.layers.push(aboutKDEPage)
                }
            }
        }
    }
}
