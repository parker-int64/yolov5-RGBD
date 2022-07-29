import QtQuick 2.3
import QtQuick.Controls 2.3
Item {
    id: item1
    width: 800
    height: 600
    Rectangle {
        id: calibContent
        width: parent.width
        height: parent.height
        radius: 20
        color: "#f5f5f5"

        Rectangle {
            id: card1
            y: 187
            width: 200
            height: 320
            color: "#7c4dff"
            radius: 20
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.verticalCenterOffset: 0
            anchors.leftMargin: 50

            Text {
                id: stepText1
                width: 49
                color: "#ffffff"
                text: qsTr("Step 1")
                height: 25
                anchors.top: parent.top
                font.pixelSize: 18
                font.family: "Fredoka Light"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 20
            }

            Text {
                id: stepTip1
                x: 88
                width: 180
                height: 40
                color: "#ffffff"
                text: qsTr("Choose your images from local file.")
                anchors.top: stepText1.bottom
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                font.weight: Font.Bold
                font.family: "Fredoka Light"
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                id: fileSelectBtn
                x: 68
                y: 228
                text: qsTr("Select")
                anchors.bottom: parent.bottom
                font.pixelSize: 14
                font.bold: true
                font.weight: Font.Bold
                font.family: "Fredoka Light"
                highlighted: true
                flat: false
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: 20
            }
        }

        Rectangle {
            id: card2
            y: 187
            width: 200
            height: 320
            color: "#7c4dff"
            radius: 20
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: card1.right
            anchors.verticalCenterOffset: 0
            anchors.leftMargin: 50

            Text {
                id: stepText2
                x: -175
                width: 49
                height: 25
                color: "#ffffff"
                text: qsTr("Step 2")
                anchors.top: parent.top
                font.pixelSize: 18
                anchors.topMargin: 20
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: "Fredoka Light"
            }

            Text {
                id: stepTip2
                x: 12
                width: 180
                height: 40
                color: "#ffffff"
                text: qsTr("Choose calibration parameters.")
                anchors.top: stepText2.bottom
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 20
                font.weight: Font.Bold
                font.family: "Fredoka Light"
            }

            ComboBox {
                id: chessWidthComboBox
                x: 40
                width: 180
                height: 40
                anchors.top: stepTip2.bottom
                font.pixelSize: 14
                font.family: "Fredoka Light"
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                displayText: qsTr("Chessboard Width")
            }

            ComboBox {
                id: chessHeightComboBox
                x: 10
                width: 180
                height: 40
                anchors.top: chessWidthComboBox.bottom
                font.pixelSize: 14
                anchors.topMargin: 10
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                displayText: qsTr("Chessboard Height")
                font.family: "Fredoka Light"
            }

            ComboBox {
                id: chessSizeComboBox
                x: 10
                width: 180
                height: 40
                anchors.top: chessHeightComboBox.bottom
                font.pixelSize: 14
                anchors.horizontalCenterOffset: 0
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                displayText: qsTr("Chessboard Size")
                font.family: "Fredoka Light"
            }
        }

        Rectangle {
            id: card3
            y: 187
            width: 200
            height: 320
            color: "#7c4dff"
            radius: 20
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: card2.right
            anchors.verticalCenterOffset: 0
            anchors.leftMargin: 50

            Text {
                id: stepText3
                x: -175
                y: -120
                width: 49
                height: 25
                color: "#ffffff"
                text: qsTr("Step 2")
                anchors.top: parent.top
                font.pixelSize: 18
                anchors.horizontalCenterOffset: 0
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: "Fredoka Light"
            }

            Text {
                id: stepTip3
                x: -240
                width: 180
                height: 40
                color: "#ffffff"
                text: qsTr("All done now! Let's start calibration!")
                anchors.top: stepText3.bottom
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                font.weight: Font.Bold
                font.family: "Fredoka Light"
            }

            Button {
                id: calibStartBtn
                text: qsTr("Start")
                anchors.bottom: parent.bottom
                font.pixelSize: 14
                font.bold: true
                font.family: "Fredoka Light"
                highlighted: true
                anchors.bottomMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
    Behavior on visible {
        PropertyAnimation{
            target: calibContent
            property: "opacity"
            from: 0
            to: 1
            duration: 600
            easing.type: Easing.InExpo
        }
    }


}
