import QtQuick 2.3
import QtQuick.Window 2.3
import QtQuick.Controls 2.3

Window{

    id: appMainWindow
    // Load fonts
    FontLoader {
        id: fredokaFont;
        source: "qrc:/assets/Fredoka-VariableFont_wdth,wght.ttf"
    }
    width: 1280
    height: 800
    minimumWidth: 1280
    minimumHeight: 800
    visible: true
    title: qsTr("Yolo RGBD Demo")

    property bool unfold: false;

    Item{
        id: container
        x: 0
        y: 0
        width: parent.width
        height: parent.height

        // Basic background color
        Rectangle {
            id: backgroundRect
            width: parent.width
            height: parent.height
            color: "#6843d1"
        }


        Rectangle{
            id: barRect;
            width: unfold ? 240 : 64;
            height: 380;
            radius: 10;
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 20
            z: 1
            y: 10
            color: "#f5f5f5";
            clip: true;

            Behavior on width{
                NumberAnimation{
                    duration: 300;
                }
            }

            ListModel{
                id: appModel;
                ListElement{
                    name: qsTr("Home");
                    icon: "qrc:/assets/home.png";

                }
                ListElement{
                    name: qsTr("Demo");
                    icon: "qrc:/assets/camera.png";
                }
                ListElement{
                    name: qsTr("Calibrate");
                    icon: "qrc:/assets/dashboard.png";
                }
                ListElement{
                    name: qsTr("Help");
                    icon: "qrc:/assets/help.png";
                }
                ListElement{
                    name: qsTr("Settings");
                    icon: "qrc:/assets/settings.png";
                }


            }

            Component {
                id: appDelegate;
                Rectangle {
                    id: delegateBackground;
                    width: barRect.width;
                    height: 48;
                    radius: 5;
                    color: "#00000000";
                    // Show icons in sidebar
                    Image {
                        id: imageIcon;
                        width: 24;
                        height: 24;
                        anchors.verticalCenter: parent.verticalCenter;
                        anchors.left: parent.left;
                        anchors.leftMargin: 18;
                        mipmap: true;
                        source: icon;
                    }
                    // Show text in sidebar
                    Text {
                        anchors.left: imageIcon.right;
                        anchors.leftMargin: 40;
                        anchors.verticalCenter: imageIcon.verticalCenter;
                        color: "#6843d1"
                        text: name;
                        font{family: "微软雅黑"; pixelSize: 20;}
                    }
                    // Handle Mouse event
                    MouseArea{
                        anchors.fill: parent;
                        hoverEnabled: true;
                        onEntered: delegateBackground.color = "#10000000";
                        onExited: delegateBackground.color = "#00000000";
                        onClicked: {
                            // switch pages
                            switch(name){
                                case 'Demo':
                                    homePage.visible = false
                                    demoPage.visible = true
                                    calibratePage.visible = false
                                    infoPage.visible = false
                                    settingsPage.visible = false
                                break
                                case 'Calibrate':
                                    homePage.visible = false
                                    demoPage.visible = false
                                    calibratePage.visible = true
                                    infoPage.visible = false
                                    settingsPage.visible = false
                                break
                                case 'Help':
                                    homePage.visible = false
                                    demoPage.visible = false
                                    calibratePage.visible = false
                                    infoPage.visible = true
                                    settingsPage.visible = false
                                break
                                case 'Settings':
                                    homePage.visible = false
                                    demoPage.visible = false
                                    calibratePage.visible = false
                                    infoPage.visible = false
                                    settingsPage.visible = true
                                break
                                default:
                                    homePage.visible = true
                                    demoPage.visible = false
                                    calibratePage.visible = false
                                    infoPage.visible = false
                                    settingsPage.visible = false
                            }

                        }
                    }
                }
            }

            GridView{
                id: appGrid;
                width: 160;
                height: parent.height;
                anchors.left: parent.left;
                anchors.top: parent.top;
                anchors.topMargin: 12;
                model: appModel;
                delegate: appDelegate;
                cellWidth: width;
                cellHeight: 60;
            }
        }

        // fold  / unfold
        Rectangle{
            width: 34;
            height: width
            radius: width/2
            color: "#f5f5f5"
            border.color: "#6843d1"
            border.width: 5
            anchors.left: barRect.right
            z: 1
            anchors.leftMargin: -width/2
            anchors.verticalCenter: barRect.verticalCenter
            Image {
                width: 24
                height: 24
                anchors.centerIn: parent
                mipmap: true
                //Rotate 180
                rotation: unfold? 180:0
                source: "qrc:/assets/arrow.png"
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    unfold = !unfold
                }
            }
        }




        Rectangle {
            id: contentPageBackground
            width: parent.width - ( barRect.width + 40 )
            height: parent.height - 50
            color: "#f5f5f5"
            radius: 20
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: barRect.right
            anchors.leftMargin: 5


            // Show different page

            HomePage {
                id: homePage
                visible: true
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

            }

            DemoPage {
                id: demoPage
                visible: false
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            CalibratePage {
                id: calibratePage
                visible: false
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            InfoPage {
                id: infoPage
                visible: false
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            SettingsPage{
                id: settingsPage
                visible: false
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }




        }
    }
}


