import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout{
    spacing: 10
    Layout.alignment: Qt.AlignHCenter

    Row {
        spacing: 25
        anchors.centerIn: parent

        ButtonGroup {
            id: reading_select_group

            onClicked: (button) =>{
                backend.onReadingSelectionRadioButtonClicked(button.value)
            }
        }

        component StyledRadio : RadioButton {
            id: control
            font.pixelSize: 18
            ButtonGroup.group: reading_select_group

            implicitWidth: contentItem.implicitWidth + leftPadding + rightPadding

            contentItem: Text {
                text: control.text
                font: control.font
                color: control.checked ? "#ffffff" : "#99dcf0f5"
                verticalAlignment: Text.AlignVCenter

                leftPadding: control.indicator.width + control.spacing

                Behavior on color { ColorAnimation { duration: 200 } }
            }

            indicator: Rectangle {
                implicitWidth: 20
                implicitHeight: 20

                x: control.leftPadding
                y: parent.height / 2 - height / 2

                radius: width / 2
                border.width: control.checked ? 3 : 2
                border.color: control.checked ? "#00a2ff" : "#666"
                color: "transparent"

                Rectangle {
                    width: 12
                    height: 12
                    anchors.centerIn: parent
                    radius: width / 2
                    color: "#00a2ff"
                    visible: control.checked

                    scale: control.checked ? 1 : 0
                    Behavior on scale { NumberAnimation { duration: 150; easing.type: Easing.OutBack } }
                }
            }
        }

        StyledRadio{ text: "Carbon Dioxide"; checked: true; property int value: 0 }
        StyledRadio{ text: "Temperature"; property int value: 1 }
        StyledRadio{ text: "Humidity"; property int value: 2 }
    }
}
