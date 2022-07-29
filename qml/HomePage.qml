import QtQuick 2.3
import QtQuick.Controls 2.3
import QtCharts 2.3


Item {
    width: 800
    height: 600
    Rectangle {
        id: homeContent
        x: 0
        y: 0
        width: parent.width
        height: 600
        radius: 20
        color: "#f5f5f5"

        BusyIndicator {
            id: busyIndicator
            x: 35
            y: 22
            width: 100
            height: 100
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -199
            anchors.horizontalCenterOffset: 300
            z: 0
            anchors.horizontalCenter: parent.horizontalCenter
            running: false
        }

        Connections {
            target: controller
            function onCpuUsageChanged() {
                localCpuUsage.progress  = controller.getCpuUsage()
                localCpuUsage.requestPaint()
            }

            function onNvidiaGpuUsageChanged() {
                localGpuUsage1.progress = controller.getNvidiaGpuUsage()
                localGpuUsage1.requestPaint()
            }

            function onMemUsageChanged() {
                localCpuUsage.progress = controller.getMemUsage()
                localMemUsage.requestPaint();
            }
            Component.onDestruction: {
                controller.monitorThreadFinished()
            }
        }

        Rectangle {
            id: memCard
            x: 325
            width: 150
            height: 150
            color: "#fe6196"
            radius: 20
            anchors.top: cpuCard.bottom
            anchors.topMargin: 10

            CircleProgress {
                id: localMemUsage
                x: 598
                y: 83
                anchors.centerIn: parent
                arcWidth: 5
                radius: 30
                anchors.verticalCenterOffset: -27
                anchors.horizontalCenterOffset: 1
                progress: controller.memUsage

            }

            Text {
                id: localMemUsageText
                x: 74
                y: 20
                color: "#ffffff"
                text: qsTr("Memory")
                anchors.top: localMemUsage.bottom
                font.pixelSize: 14
                anchors.horizontalCenter: localMemUsage.horizontalCenter
                anchors.topMargin: 10
                font.family: "Fredoka Light"
                font.weight: Font.Bold
            }

            Text {
                id: totalMem
                x: 94
                color: "#ffffff"
                text: qsTr("Total Memory: " + statusMonitor.memory() + " GB")
                anchors.top: localMemUsageText.bottom
                font.pixelSize: 12
                font.bold: false
                font.weight: Font.Bold
                font.family: "Fredoka Light"
                anchors.horizontalCenter: localMemUsageText.horizontalCenter
                anchors.topMargin: 10
            }
        }



        Rectangle {
            id: rectangle
            x: 35
            y: 31
            width: 200
            height: 300
            color: "#46b2ff"
            radius: 20
            anchors.right: cpuCard.left
            anchors.rightMargin: 25

            Image {
                id: platformImage
                x: 50
                y: 46
                width: 100
                height: 100
                mipmap: true
                source: "qrc:/assets/ms_windows.png"
            }

            Text {
                id: machineName
                text: statusMonitor.localMachineName()
                x: 88
                color: "#ffffff"
                anchors.top: platformImage.bottom
                font.pixelSize: 14
                anchors.horizontalCenterOffset: 1
                anchors.horizontalCenter: platformImage.horizontalCenter
                font.family: "Fredoka Light"
                font.weight: Font.Bold
                anchors.topMargin: 35
            }

            Text {
                id: osName
                text: statusMonitor.osVersion()
                x: 88
                y: 223
                color: "#ffffff"
                anchors.top: machineName.bottom
                font.pixelSize: 14
                font.weight: Font.Bold
                anchors.horizontalCenterOffset: 1
                font.family: "Fredoka Light"
                anchors.horizontalCenter: machineName.horizontalCenter
                anchors.topMargin: 20
            }
        }

        Text {
            x: 296
            y: 64
            text: qsTr("System Information")
            anchors.bottom: infoText.top
            anchors.bottomMargin: 11
            anchors.horizontalCenterOffset: 0;
            font.family: "Fredoka Light"
            font.pointSize: 18
            font.weight: Font.Bold
        }


        Text {
            id: infoText
            text: qsTr("CPU:  " + statusMonitor.cpuType() + "\n" +
                       "GPU0: " + statusMonitor.intelGPU() + "\n" +
                       "GPU1: " + statusMonitor.nvidiaGPU())
            anchors.verticalCenterOffset: 181
            anchors.horizontalCenterOffset: 0
            anchors.centerIn: parent
            font.family: "Fredoka Light"
            font.pointSize: 12
            font.weight: Font.Bold

        }


        Rectangle {
            id: cpuCard
            x: 276
            y: 31
            width: 150
            height: 140
            color: "#fecc50"
            radius: 20
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            CircleProgress {
                id: localCpuUsage
                x: 59
                width: 75
                height: 78
                arcWidth: 5
                radius: 30
                anchors.top: parent.top
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 21
                progress: controller.cpuUsage

            }

            Text {
                id: cpuUsageText
                x: 77
                y: 176
                width: 47
                height: 15
                color: "#ffffff"
                text: qsTr("CPU")
                anchors.top: localCpuUsage.bottom
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: localCpuUsage.horizontalCenter
                font.family: "Fredoka Light"
                font.weight: Font.Bold
                anchors.topMargin: 10
            }
        }

        Rectangle {
            id: gpuCard0
            y: 31
            width: 145
            height: 140
            color: "#202529"
            radius: 20
            anchors.left: cpuCard.right
            anchors.leftMargin: 15

            Text {
                id: localGpuUsageText0
                x: 49
                width: 47
                height: 15
                color: "#ffffff"
                text: qsTr("GPU0")
                anchors.top: clickInfo.bottom
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenterOffset: 0
                anchors.topMargin: 20
                font.family: "Fredoka Light"
                font.weight: Font.Bold
            }

            Text {
                id: clickInfo
                x: 8
                y: 59
                width: 129
                height: 23
                color: "#ffffff"
                text: qsTr("Click to know more")
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 14
                anchors.horizontalCenter: parent.horizontalCenter
                font.weight: Font.Bold
                font.family: "Fredoka Light"
            }

            Text {
                id: statusInfo
                x: 23
                y: 24
                width: 99
                height: 23
                color: "#ffffff"
                text: "Unavailable"
                anchors.bottom: clickInfo.top
                font.pixelSize: 18
                anchors.bottomMargin: 11
                font.weight: Font.Bold
                font.family: "Fredoka Light"
            }

            MouseArea {
                id: mouseArea
                width: parent.width
                height: parent.height

                onClicked: {
                    intelGpuDialogPopup.open()
                    intelGpuDialog.innerText = statusMonitor.intelGpuInfo()
                }
            }
        }

        Rectangle {
            id: gpuCard1
            y: 181
            width: 145
            height: 150
            color: "#89d400"
            radius: 20
            anchors.left: memCard.right
            anchors.leftMargin: 15

            CircleProgress {
                id: localGpuUsage1
                x: 59
                width: 75
                height: 78
                radius: 30
                anchors.top: parent.top
                anchors.horizontalCenterOffset: 0
                arcWidth: 5
                anchors.topMargin: 21
                anchors.horizontalCenter: parent.horizontalCenter
                progress: controller.nvidiaGpuUsage
            }

            Text {
                id: localGpuUsageText1
                x: 49
                width: 47
                height: 15
                color: "#ffffff"
                text: qsTr("GPU1")
                anchors.top: localGpuUsage1.bottom
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenterOffset: 0
                anchors.topMargin: 10
                font.family: "Fredoka Light"
                font.weight: Font.Bold
            }

        }
            Popup {
                id: intelGpuDialogPopup
                width: 680
                height: 400
                x: (parent.width - width) / 2
                y: (parent.height - height) / 2
                modal: true
                focus: true
                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
                padding: 0

                CustomDialog {
                    id: intelGpuDialog
                    dialogWidth: parent.width
                    dialogHeight: parent.height

                    okButtonEnable: true
                    expectContentWidth: parent.width - 50
                    expectContentHeight: 3400
                    anchors.fill: parent
                }
            }
    }





    Behavior on visible {
        PropertyAnimation{
            target: homeContent
            property: "opacity"
            from: 0
            to: 1
            duration: 600
            easing.type: Easing.InExpo
        }
    }

}
