import QtQuick 2.15
import QtCharts 2.14
import QtQuick.Controls 2.14

import QtQuick.Dialogs 1.3
import Qt.labs.platform 1.0

Item {
    id: item1
    width: 980
    height: 800

    Rectangle {
        id: demoContent
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        radius: 20
        color: "transparent"


        Rectangle{
            id: canvas
            width: 660
            height: 500
            color: "transparent"
            border.color: "black"
            border.width: 3
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenterOffset: 0

            Image {
                id: imageRGB
                y: 232
                width: 640
                height: 480
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/assets/videocam.png"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenterOffset: 0
                fillMode: Image.PreserveAspectFit
                cache: false
                asynchronous: false
                sourceSize.width: 640
                sourceSize.height: 480
                // Reload image
                function reload(){
                    source = ""
                    source = "image://live/image/" + Date.now()

                }
                // Set default image
                function setDefault(){
                    source = "qrc:/assets/videocam.png"
                }
            }
        }

        Rectangle {
            id: controlPanel
            width: 271
            height: 574
            color: "#ffffff"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: canvas.right
            anchors.leftMargin: 20

            property var paraList : {
                "camera_index": 0,
                "resolution": {
                    "width": 640,
                    "height": 480,
                },
                "infer_device": "Intel CPU",
                "yolo_network_directory": "./",
                "yolo_model_size": "nano"
            }

            function stringifyAndSend(paraList){
                var jsonString = JSON.stringify(paraList)
                console.log(jsonString)
                console.log("Send to C++")
                // balabala send!
                utility.parseJSValue(paraList)
            }


            Text {
                id: cameraText
                text: qsTr("Camera Index")
                anchors.left: parent.left
                anchors.top: parent.top
                font.pixelSize: 14
                font.family: "Fredoka Light"
                anchors.topMargin: 20
                anchors.leftMargin: 10
            }

            ComboBox {
                property int camIndex: 0
                id: camIndexSelect
                width: 120
                anchors.verticalCenter: cameraText.verticalCenter
                anchors.left: cameraText.right
                anchors.leftMargin: 42
                font.pixelSize: 12
                font.family: "Fredoka Light"
                model:[0,1,2,3,4,5]
                onDisplayTextChanged: {
                    switch(displayText){
                    case '1':
                        camIndex = 1
                        break;
                    case '2':
                        camIndex = 2
                        break;
                    case '3':
                        camIndex = 3
                        break;
                    case '4':
                        camIndex = 4
                        break;
                    case '5':
                        camIndex = 5
                        break;
                    default:
                        camIndex = 0
                        console.log("Default camera index is 0 !")
                        break;
                    }
                    // Init our json data here
                    controlPanel.paraList["camera_index"] =  camIndex

                }

            }

            Text {
                id: resText
                text: qsTr("Resolution")
                anchors.left: parent.left
                anchors.top: cameraText.bottom
                font.pixelSize: 14
                font.family: "Fredoka Light"
                anchors.leftMargin: 10
                anchors.topMargin: 40
            }

            ComboBox {
                id: resSelect
                width: 120
                anchors.verticalCenter: resText.verticalCenter
                anchors.horizontalCenter: camIndexSelect.horizontalCenter
                font.pixelSize: 12
                font.family: "Fredoka Light"
                model:["640×480"]
                onDisplayTextChanged: {
                    controlPanel.paraList["resolution"]["width"] = 640
                    controlPanel.paraList["resolution"]["height"] = 480

                }
            }

            Text {
                id: inferDeviceText
                text: qsTr("Inference Device")
                anchors.left: parent.left
                anchors.top: resText.bottom
                font.pixelSize: 14
                font.family: "Fredoka Light"
                anchors.leftMargin: 10
                anchors.horizontalCenterOffset: 18
                anchors.topMargin: 40
            }

            ComboBox {
                id: inferDevide
                width: 120
                anchors.verticalCenter: inferDeviceText.verticalCenter
                anchors.horizontalCenter: resSelect.horizontalCenter
                font.pixelSize: 12
                font.family: "Fredoka Light"
                model: ["Intel CPU", "Nvidia GPU"]
                onDisplayTextChanged: {
                    controlPanel.paraList["infer_device"] = displayText
                }
            }

            Text {
                id: yolov5NetworkText
                text: qsTr("Yolov5 network file directory")
                anchors.left: parent.left
                anchors.top: inferDeviceText.bottom
                font.pixelSize: 14
                font.family: "Fredoka Light"
                anchors.leftMargin: 10
                anchors.topMargin: 40
            }

            Text {
                id: yolov5NetworkDir
                width: 220
                height: 20
                anchors.left: parent.left
                anchors.top: yolov5NetworkText.bottom
                anchors.leftMargin: 10
                anchors.topMargin: 40
                font.pixelSize: 12
                font.family: "Fredoka Light"
                text: qsTr("Path to a directory...")
                elide: Text.ElideRight
                leftPadding: 5
                verticalAlignment: Text.AlignVCenter
                Rectangle {
                    width: parent.width
                    height: parent.height
                    border.width: 1
                    border.color: "black"
                    color: "transparent"
                }
            }

            Image {
                id: selectFile
                width: 24
                height: 24
                anchors.verticalCenter: yolov5NetworkDir.verticalCenter
                anchors.left: yolov5NetworkDir.right
                source: "qrc:/assets/file_select.png"
                anchors.leftMargin: 10
                fillMode: Image.PreserveAspectFit

                MouseArea{
                    width: parent.width
                    height: parent.height
                    onClicked: {
                        networkFolderDialog.open()
                    }
                }
            }
            FolderDialog {
                id: networkFolderDialog
//                property string url: ""
                onAccepted: {
                    var url = networkFolderDialog.folder
                    yolov5NetworkDir.text = url.toString().slice(8)
                    controlPanel.paraList["yolo_network_directory"] = url.toString().slice(8)
                }
                onRejected: {
                    yolov5NetworkDir.text = qsTr("Must provide a valid folder path...")
                }
            }

            Text {
                id: modelSizeText
                text: qsTr("Model Size")
                anchors.left: parent.left
                anchors.top: yolov5NetworkDir.bottom
                font.pixelSize: 14
                font.family: "Fredoka Light"
                anchors.leftMargin: 10
                anchors.topMargin: 50
            }

            ComboBox {
                id: modelSizeSelect
                width: 120
                anchors.verticalCenter: modelSizeText.verticalCenter
                anchors.horizontalCenter: inferDevide.horizontalCenter
                font.pixelSize: 12
                font.family: "Fredoka Light"
                model: ["nano", "tiny", "small", "medium", "large"]
                onDisplayTextChanged: {
                    controlPanel.paraList["yolo_model_size"] = displayText
                }
            }

            Button {
                id: saveParameterBtn
                text: qsTr("Save and init Engine")
                anchors.top: modelSizeText.bottom
                font.family: "Fredoka Light"
                font.pixelSize: 12
                highlighted: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 40
                flat: false
                property bool validParameter : false
                onClicked: {
                    // This button slot function:
                    // 1. stringify js/qml object file and send to C++ backend
                    // 2. check the parameters, return status from C++
                    controlPanel.stringifyAndSend(controlPanel.paraList)

                    checkParameter()
                }
                function checkParameter(){
                    validParameter = utility.getEngineStatus()
                    if(validParameter){
                        console.log("Successfully found the files, you may begin to detect")
                        startCaptureBtn.enabled = true
                        startYoloBtn.enabled = true
                        startDemoBtn.enabled = true
                    } else {
                        console.log("Error: Requested files cannot be found.")
                        startYoloBtn.enabled = false
                        startDemoBtn.enabled = false
                        startCaptureBtn.enabled = false
                    }
                }
            }

            MessageDialog {
                id: messageDialog
                title: qsTr("Error Occurs!")
                text: utility.errorMessage

                buttons: MessageDialog.Ok
                onOkClicked: {
                    messageDialog.close()
                }
            }

            Text {
                id: helpText
                width: 231
                height: 80
                text: qsTr("You should set the parameters here and click 'SAVE AND INIT ENGINE' button before you start running the program on the left.")
                anchors.left: parent.left
                anchors.top: saveParameterBtn.bottom
                font.pixelSize: 12
                font.family: "Fredoka Light"
                anchors.leftMargin: 20
                anchors.topMargin: 30
                wrapMode: Text.Wrap
            }
        }

        ToolBar {
            id: toolBar
            width: 480
            height: 60
            position: ToolBar.Footer
            anchors.top: canvas.bottom
            anchors.horizontalCenter: canvas.horizontalCenter
            anchors.topMargin: 40

            Button {
                id: startCaptureBtn
                height: 50
                enabled: false
                text: qsTr("Start Capture")
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                font.pixelSize: 14
                font.family: "Fredoka Light"
                anchors.leftMargin: 10
                property bool start_capture: false
                onClicked: {
                    start_capture = !start_capture
                    if(start_capture){
                        console.log("Start Capture...")
                        text = qsTr("End Capture")
                        controller.imageThreadStart()
                    } else {
                        console.log("End Capture...")
                        text = qsTr("Start Capture")
                        controller.imageThreadFinished()
                        delayTimer.start() // set to default img
                    }
                }

            }

            Button {
                id: startYoloBtn
                height: 50
                text: qsTr("Start Yolo Detection")
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: startCaptureBtn.right
                font.pixelSize: 14
                font.family: "Fredoka Light"
                flat: false
                anchors.leftMargin: 20
                enabled: false                  // default false
                property bool run_yolo: false
                onClicked: {
                    run_yolo = !run_yolo
                    if(run_yolo){
                        console.log("Starting Yolo...")
                        text = qsTr("End Yolo Detection")
                    } else {
                        console.log("Ending Yolo...")
                        text = qsTr("Start Yolo Detection")
                    }

                    controller.startYoloDetect()
                }
            }

            Button {
                id: startDemoBtn
                height: 50
                text: qsTr("Start Demo")
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: startYoloBtn.right
                anchors.leftMargin: 20
                font.pixelSize: 14
                font.family: "Fredoka Light"
                enabled: false
                onClicked: {

                }
            }
        }
    }

    Behavior on visible {
        PropertyAnimation{
            target: demoContent
            property: "opacity"
            from: 0
            to: 1
            duration: 600
            easing.type: Easing.InExpo
        }
    }

    Connections {
        target: liveImageProvider
        function onImageChanged() {
            imageRGB.reload()
        }
        Component.onDestruction: {
            controller.imageThreadFinished()
        }
    }

    Connections {
        target: utility
        function onErrorMessageChanged() {
            messageDialog.text = utility.getErrorMessage()
            messageDialog.open()
            saveParameterBtn.validParameter = false
        }
    }

    Timer {
        id: delayTimer
        interval: 200
        repeat: false
        triggeredOnStart: true
        onTriggered: {
            imageRGB.setDefault()
        }
    }
}



/*##^##
Designer {
    D{i:0;formeditorZoom:1.25}D{i:21}
}
##^##*/
