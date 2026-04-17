import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout{
    id: current_data_card
    spacing: 5
    Layout.fillWidth: true
    Layout.fillHeight: true

    property int co2: 0
    property double temperature: 0
    property double humidity: 0
    property string last_reading_time: ""
    property string range_description: "Waiting for data..."

    Connections{
        target: backend

        function onUpdateCurrentData(c, t, h, time, desc){
            current_data_card.co2 = c
            current_data_card.temperature = t
            current_data_card.humidity = h
            current_data_card.last_reading_time = time
            current_data_card.range_description = desc
        }
    }

    // Main header
    Text{
        font.pixelSize: 20
        color: "white"
        text: "Current measurements"

        Layout.alignment: Qt.AlignHCenter
    }

    Item {
        id: co2_gauge
        Layout.preferredWidth: current_data_card.width * 0.9
        Layout.preferredHeight: current_data_card.height * 0.45
        Layout.alignment: Qt.AlignHCenter

        property int value: co2

        onValueChanged: canvas.requestPaint()
        onWidthChanged: canvas.requestPaint()
        onHeightChanged: canvas.requestPaint()

        // CO2 PPM gauge
        Canvas {
            id: canvas
            anchors.fill: parent

            onPaint: {
                const minValue = 350
                const maxValue = 2100;
                let value = co2_gauge.value;

                // helper function to calculate dial zones
                function t(ppm) {
                    return (ppm - minValue) / (maxValue - minValue)
                }

                if (value > maxValue)
                    value = maxValue
                else if (value < minValue)
                    value = minValue

                let ctx = getContext("2d")
                ctx.clearRect(0, 0, width, height)

                let cx = width / 2
                let cy = height / 2
                let radius = Math.min(width, height) / 2 - 15

                let progress = (value - minValue) / (maxValue - minValue)

                // // make the gap be at the bottom
                let startAngle = 35 * (Math.PI / 180) + Math.PI / 2
                let endAngle = 325 * (Math.PI / 180) + Math.PI / 2
                let angle = startAngle + progress * (endAngle - startAngle)
                let sweep = endAngle - startAngle

                let zones = [
                    { t: t(350),  color: "darkgreen" }, // Excellent
                    { t: t(450),  color: "green" }, // Good
                    { t: t(600),  color: "yellow" }, // Normal
                    { t: t(900),  color: "orange" }, // Poor
                    { t: t(1200), color: "red" }, // Bad
                    { t: t(1500), color: "darkred" }, // Awful
                    { t: t(2000), color: "black" }, // Dangerous
                    { t: t(2100), color: "black" }, // Cap
                ]

                for (var i = 0; i < zones.length - 1; i++)
                {
                    let a1 = startAngle + zones[i].t * sweep
                    let a2 = startAngle + zones[i + 1].t * sweep

                    ctx.beginPath()
                    ctx.arc(cx, cy, radius, a1, a2)
                    ctx.lineWidth = 12
                    ctx.strokeStyle = zones[i].color
                    ctx.lineCap = "butt"
                    ctx.stroke()
                }

                let arrowLength = 18
                let arrowWidth = 7

                // tip of arrow on arc
                let ax = cx + Math.cos(angle) * (radius - 10) // offset to move arrow closer to the center
                let ay = cy + Math.sin(angle) * (radius - 10) // offset to move arrow closer to the center

                let dx = Math.cos(angle)
                let dy = Math.sin(angle)

                let px = -dy
                let py = dx

                ctx.beginPath()

                ctx.moveTo(ax, ay)

                ctx.lineTo(
                    ax - dx * arrowLength + px * arrowWidth,
                    ay - dy * arrowLength + py * arrowWidth
                )

                ctx.lineTo(
                    ax - dx * arrowLength - px * arrowWidth,
                    ay - dy * arrowLength - py * arrowWidth
                )

                ctx.closePath()

                ctx.fillStyle = "white"
                ctx.fill()
            }
        }

        // Center text
        Column {
            anchors.centerIn: parent
            spacing: 2

            Text {
                text: co2_gauge.value
                font.pixelSize: 24
                font.bold: true
                color: "white"
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                text: "PPM"
                font.pixelSize: 16
                color: "#dcf0f5"
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    Text{
        font.pixelSize: 16
        leftPadding: 4
        color: "white"
        Layout.fillWidth: true
        wrapMode: Text.WordWrap
        text: range_description
    }

    Text{
        font.pixelSize: 16
        leftPadding: 4
        color: "#dcf0f5"
        text: "Last reading: " + (last_reading_time.length == 0 ? "No data" : last_reading_time)
    }

    Text{
        font.pixelSize: 16
        leftPadding: 4
        color: "#dcf0f5"
        text: "Temperature: " + temperature.toFixed(1) + " °C"
    }

    Text{
        font.pixelSize: 16
        leftPadding: 4
        color: "#dcf0f5"
        text: "Humidity: " + humidity.toFixed(1) + "%"
    }

    // Esnures full width and positions header at the top
    Item{
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
}
