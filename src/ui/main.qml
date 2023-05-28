// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Scene3D 2.15
import Qt3D.Core 2.15
import Qt3D.Render 2.15 as Render
import Qt3D.Input 2.15
import Qt3D.Extras 2.15
import org.kde.kirigami 2.20 as Kirigami
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kmodelviewer 1.0

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

    pageStack.initialPage: Kirigami.Page {
        title: i18nc("@title:window 3D Viewport", "Viewport")

        padding: 0

        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            text: i18nc("@info:placeholder Viewport placeholder", "No model loaded")
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
                            clearColor: "black"
                        }
                    },
                    InputSettings {}
                ]

                Render.Camera {
                    id: mainCamera
                    position: Qt.vector3d(10, 10, 10)
                    viewCenter: Qt.vector3d(0, 0, 0)
                    fieldOfView: 60
                }

                OrbitCameraController {
                    camera: mainCamera
                    linearSpeed: 5
                    lookSpeed: 180
                }

                Entity {
                    id: sphereObject

                    components: [
                           Render.SceneLoader {
                                id: sphereMesh
                                source: Qt.resolvedUrl("file://" + appWindow.currentlyLoadedFile)
                           }
                       ]
                }

                Entity {
                       components: [
                           Transform {
                               translation: Qt.vector3d(15, 15, 15)
                           },

                           Render.PointLight {
                               enabled: true
                               color: "red"
                               intensity: 1.0
                           }
                       ]
                }
            }
        }
    }
}
