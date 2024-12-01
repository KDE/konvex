// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Window
import QtQuick.Scene3D
import Qt3D.Core
import Qt3D.Render as Render
import Qt3D.Input
import Qt3D.Extras
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
            visible: sphereMesh.status !== Render.Mesh.Ready
        }

        Scene3D {
            id: scene3d
            anchors.fill: parent
            aspects: ["input", "logic"]
            cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

            Entity {
                components: [
                    Render.RenderSettings {
                        activeFrameGraph: ForwardRenderer {
                            camera: mainCamera
                            clearColor: "transparent"
                        }
                    },
                    InputSettings {}
                ]

                Render.Camera {
                    id: mainCamera
                    position: Qt.vector3d(10, 10, 10)
                    viewCenter: Qt.vector3d(0, 0, 0)
                    fieldOfView: 60
                    farPlane: 100000
                }

                OrbitCameraController {
                    camera: mainCamera
                    lookSpeed: 180

                    // Qt 3D is weird, and doesn't give us the bounding box of a mesh (WHY?)
                    // so instead, we will try to guess based on camera distance
                    linearSpeed: {
                        let biggestComponent = Math.max(mainCamera.position.x, mainCamera.position.y, mainCamera.position.z);

                        return 10 * biggestComponent;
                    }
                }

                Entity {
                    id: sphereObject

                    components: [
                           Render.SceneLoader {
                                id: sphereMesh
                                source: appWindow.currentlyLoadedFile

                                onStatusChanged: {
                                    if(status == Render.SceneLoader.Ready)
                                        mainCamera.viewAll()
                                }
                           }
                       ]
                }

                Entity {
                       components: [
                           Render.DirectionalLight {
                               enabled: true
                               color: "red"
                               intensity: 1.0
                               worldDirection: Qt.vector3d(0.5, -1, 1)
                           }
                       ]
                }
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
