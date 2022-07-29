import QtQuick 2.3


Canvas {
    property color arcColor: "#148014"
    property color arcBackgroundColor: "#FFFFFF"
    property int arcWidth: 2
    property int progress: 0
    property real radius: 100
    property bool anticlockwise: false

    id: canvas
    width: 2 * radius + arcWidth
    height: 2 * radius + arcWidth

    Text{
        color: "#ffffff"
        anchors.centerIn: parent
        text: progress + "%"
        font.pointSize: 14
        font.weight: Font.Bold
        font.family: "Fredoka Light"

    }

    onPaint: {
        // draw a circle first
        var ctx = getContext("2d")
        ctx.clearRect(0, 0, width, height)
        ctx.beginPath()
        ctx.strokeStyle = arcBackgroundColor
        ctx.lineWidth = arcWidth
        ctx.arc(width / 2, height / 2, radius, 0, Math.PI*2, anticlockwise)
        ctx.stroke()

        // draw the progress bar
        var r = progress * Math.PI / 180
        ctx.beginPath()
        ctx.strokeStyle = arcColor
        ctx.lineWidth = arcWidth

//        ctx.arc(width / 2, height / 2, radius, 0 - 90 * Math.PI / 180, r - 90 * Math.PI / 180, anticlockwise)
        var progressEnd = (progress * 3.6 - 90) * Math.PI / 180
        if(progress >= 0 && progress < 50){
            ctx.strokeStyle = "#00e676"  // Green
        } else if(progress >= 50 && progress < 80){
            ctx.strokeStyle = "#6843d1"  // Blue
        } else if (progress >= 80 && progress <= 100){
            ctx.strokeStyle = "#ff1744"  // Red
        }

        ctx.arc(width / 2, height / 2, radius, 0 - 90 * Math.PI / 180, progressEnd, anticlockwise)
        ctx.stroke()
    }
}
