import QtQuick 2.8
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
Rectangle{
    // Bar color
    width: 480;
    height: 640;
    color: "#6843d1";


    property bool unfold: false;

    Rectangle{
        id: barRect;
        width: unfold ? 240 : 64;
        height: 384;
        radius: 10;
        x: 10
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
                name: "Home";
                icon: "qrc:/image/home.png";
            }
            ListElement{
                name: "Profile";
                icon: "qrc:/image/user.png";
            }
            ListElement{
                name: "Message";
                icon: "qrc:/image/msg.png";
            }
            ListElement{
                name: "Help";
                icon: "qrc:/image/help.png";
            }
            ListElement{
                name: "Setting";
                icon: "qrc:/image/setting.png";
            }
            ListElement{
                name: "Sign Out";
                icon: "qrc:/image/exit.png";
            }
        }

        Component{
            id: appDelegate;
            Rectangle {
                id: delegateBackground;
                width: barRect.width;
                height: 48;
                radius: 5;
                color: "#00000000";
                //显示图标
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
                //显示APP文字
                Text {
                    anchors.left: imageIcon.right;
                    anchors.leftMargin: 40;
                    anchors.verticalCenter: imageIcon.verticalCenter;
                    color: "#6843d1"
                    text: name;
                    font{family: "微软雅黑"; pixelSize: 20;}
                }
                //鼠标处理
                MouseArea{
                    anchors.fill: parent;
                    hoverEnabled: true;
                    onEntered: delegateBackground.color = "#10000000";
                    onExited: delegateBackground.color = "#00000000";
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

    // 展开/收回按钮
    Rectangle{
        width: 34;
        height: width;
        radius: width/2;
        color: "#f5f5f5";
        border.color: "#6843d1";
        border.width: 5;
        anchors.left: barRect.right;
        anchors.leftMargin: -width/2;
        anchors.verticalCenter: barRect.verticalCenter;
        Image {
            width: 24;
            height: 24;
            anchors.centerIn: parent;
            mipmap: true;
            //此处使用旋转1180度实现展开按钮图标和收回按钮图标
            rotation: unfold? 180:0;
            source: "qrc:/image/arrows.png";
        }

        MouseArea{
            anchors.fill: parent;
            onClicked: {
                unfold = !unfold;
            }
        }
    }

}
