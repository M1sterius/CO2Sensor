import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root

    property bool isSensorReady: false

    parent: Overlay.overlay
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    width: 350
    height: 400
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    background: Rectangle {
        color: "#1e1e1e"
        radius: 12
        border.width: 1
    }

    onClosed: {
        ssid_input.clear()
        password_input.clear()
        ip_input.clear()
    }

    contentItem: Item {
        anchors.fill: parent

        // Configuration window
        ColumnLayout {
            id: formLayout
            anchors.fill: parent
            anchors.margins: 20
            spacing: 15
            visible: root.isSensorReady

            Text {
                text: "Sensor Configuration"
                font.pixelSize: 22
                font.bold: true
                color: "white"
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "Provide credentials of your local network to access full functionality of the sensor."
                font.pixelSize: 14
                color: "#99dcf0f5"
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            Item { Layout.fillHeight: true }

            component ConfigInput : TextField {
                id: control

                property int minLen: 1
                property int maxLen: 5

                Layout.fillWidth: true
                color: "white"
                font.pixelSize: 16

                maximumLength: maxLen

                readonly property bool isValid: text.length >= minLen && acceptableInput

                // ASCII only validation
                validator: RegularExpressionValidator { regularExpression: /^[\x20-\x7E]*$/ }

                background: Rectangle {
                    color: "#2a2a2a"
                    radius: 6
                    border.color: !control.isValid && !control.activeFocus && control.text.length > 0 ? "#ff4444" :
                                  control.activeFocus ? "#00a2ff" : "#444"
                    border.width: 1

                    Behavior on border.color { ColorAnimation { duration: 150 } }
                }
            }

            ConfigInput {
                id: ssid_input
                minLen: 3
                maxLen: 32
                placeholderText: "WiFi SSID"
            }

            ConfigInput {
                id: password_input
                minLen: 8
                maxLen: 63
                placeholderText: "WiFi Password"
            }

            ConfigInput {
                id: ip_input
                minLen: 7
                maxLen: 15
                placeholderText: "Server IP (Optional)"

                readonly property bool isOptionalValid: text.length === 0 || isValid

                validator: RegularExpressionValidator {
                    regularExpression: /^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$/
                }
            }

            // Spacer
            Item {
                Layout.preferredHeight: 40
                Layout.fillWidth: true
            }

            // Submit Button
            Button {
                id: applyButton
                text: "Apply Settings"
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                hoverEnabled: true

                enabled: ssid_input.isValid && password_input.isValid && ip_input.isOptionalValid

                contentItem: Text {
                    text: applyButton.text
                    color: "white"
                    font.pixelSize: 16
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    opacity: applyButton.hovered ? 1.0 : 0.9
                }

                background: Rectangle {
                    id: bgRect
                    radius: 6

                    color: !applyButton.enabled ? "#333" :
                           applyButton.pressed ? "#0066cc" :
                           applyButton.hovered ? "#00a2ff" : "#1a73e8"

                    border.color: "#33b5ff"
                    border.width: applyButton.hovered && applyButton.enabled ? 1 : 0

                    opacity: applyButton.enabled ? 1.0 : 0.5

                    Behavior on color { ColorAnimation { duration: 150 } }
                }

                onClicked: {
                    backend.onSubmitSensorConfigButtonClicked(ssid_input.text, password_input.text, ip_input.text)
                    root.close()
                }
            }
        }

        // Error message window
        ColumnLayout {
            anchors.centerIn: parent
            width: parent.width * 0.8
            spacing: 20
            visible: !root.isSensorReady

            Text {
                text: "⚠️"
                font.pixelSize: 48
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "Sensor Not Connected"
                font.pixelSize: 20
                font.bold: true
                color: "#ff4444"
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "The sensor is currently not connected via serial. Please connected the sensor via USB to your PC."
                font.pixelSize: 14
                color: "#99dcf0f5"
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }
        }
    }
}
