import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    spacing: 10
    width: parent.width

    Text {
        id: sensor_connection_indicator
        font.pixelSize: 22
        color: "white"
        textFormat: Text.RichText
        verticalAlignment: Text.AlignVCenter

        property string statusColor: "red"
        property string status: "Not connected"

        text: 'Sensor: <font color="' + statusColor + '">' + status + '</font>'

        Connections {
            target: backend

            // This is a common way to silence the warning we discussed earlier
            ignoreUnknownSignals: true

            function onSensorStatusChanged(s, c) {
                sensor_connection_indicator.status = s
                sensor_connection_indicator.statusColor = c
            }
        }
    }

    // push buttons to the right
    Item { Layout.fillWidth: true }

    StyledButton {
        text: "Update graphs"
        font.pixelSize: 14
        Layout.preferredWidth: parent.width * 0.12
        onClicked: backend.onUpdateButtonClicked()
    }

    StyledButton {
        text: "Configure Sensor"
        font.pixelSize: 14
        Layout.preferredWidth: parent.width * 0.15
        onClicked: backend.onConfigureSensorButtonClicked()
    }
}
