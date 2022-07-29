import QtQuick 2.0

Item {
    width: 800
    height: 600
    Rectangle {
        id: settingsContent
        width: parent.width
        height: parent.height
        radius: 20
        color: "#f5f5f5"
        Text {
            text: qsTr("Settings Page Display")
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 12
            font.weight: Font.Bold
            font.family: "Fredoka Light"
        }
    }
    Behavior on visible {
        PropertyAnimation{
            target: settingsContent
            property: "opacity"
            from: 0
            to: 1
            duration: 600
            easing.type: Easing.InExpo
        }
    }
}
