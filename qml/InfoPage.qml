import QtQuick 2.0

Item {
    width: 800
    height: 600
    Rectangle {
        id: infoContent
        width: parent.width
        height: parent.height
        radius: 20
        color: "#f5f5f5"

        Text {
            text: qsTr("Help/Info Page Display")
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 12
            font.weight: Font.Bold
            font.family: "Fredoka Light"
        }

        Text {
            text: qsTr("ABCDEFG 界面展示  ")
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 49
            anchors.horizontalCenterOffset: 0
            font.weight: Font.Bold
            font.pointSize: 12
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Fredoka Light"
        }
    }
    Behavior on visible {
        PropertyAnimation{
            target: infoContent
            property: "opacity"
            from: 0
            to: 1
            duration: 600
            easing.type: Easing.InExpo
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}D{i:3}
}
##^##*/
