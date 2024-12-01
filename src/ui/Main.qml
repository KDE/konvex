// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Window
import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import org.kde.kirigami as Kirigami
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.konvex

Kirigami.ApplicationWindow {
    id: appWindow

    width: 640
    height: 480
    visible: true
    title: {
        let base = i18nc("@title:window Main window", "Konvex")
        if (currentlyLoadedFile.length > 0) {
            return currentlyLoadedFile + " — " + base
        } else {
            return base
        }
    }

    property string currentlyLoadedFile: ""

    Connections {
        target: Controller
        function onFileOpened(path) {
            currentlyLoadedFile = path
        }
    }

    color: "transparent"

    background: Rectangle {
        color: "transparent"
    }

    Component.onCompleted: Controller.setBlur(pageStack, true)

    pageStack.initialPage: Kirigami.Page {
        padding: 0

        background: null

        actions: [
            Kirigami.Action {
                text: i18nc("@action:intoolbar Re-center the model so it's visible in the viewport", "Re-center")
                icon.name: "snap-bounding-box-center"
                onTriggered: mainCamera.viewAll()
            },
            Kirigami.Action {
                text: i18nc("@action:intoolbar", "Settings")
                icon.name: "settings-configure"
                onTriggered: pageStack.layers.push('SettingsPage.qml')
            }
        ]

        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            text: i18nc("@info:placeholder Viewport placeholder, when no model was loaded", "No model loaded")
            explanation: i18nc("@info:placeholder Viewport placeholder, when no model was loaded", "Open a model with Konvex, or drag a model into this window.")
            visible: !model.geometry
        }

    View3D {
            anchors.fill: parent

        environment.backgroundMode: SceneEnvironment.Transparent
        environment.lightProbe: Texture {
            textureData: ProceduralSkyTextureData {
            }
        }

        Node {
            id: cameraNode
            PerspectiveCamera {
                    id: mainCamera
                z: 10
                    fieldOfView: 60
                clipFar: 100000
                clipNear: 1
                }
        }

        OrbitCameraController {
            anchors.fill: parent
            origin: cameraNode
            camera: mainCamera
        }

        Model {
            id: model

            geometry: Controller.createGeometry(currentlyLoadedFile, model)

            materials: [
                PrincipledMaterial {
                    baseColor: "red"
                    metalness: 0.0
                    roughness: 0.1
                }
            ]
            }
        }
    }

    DropArea {
        id: dropArea
        anchors.fill: parent
        onEntered: (drag) => {
            drag.accept(Qt.LinkAction);
        }
        onDropped: (drop) => {
            currentlyLoadedFile = drop.urls[0]
        }
    }
}
