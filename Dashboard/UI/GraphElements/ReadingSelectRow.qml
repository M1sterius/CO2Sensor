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

        StyledRadioButton{
            text: "Carbon Dioxide";
            checked: true
            property int value: 0
            ButtonGroup.group: reading_select_group
        }

        StyledRadioButton{
            text: "Temperature";
            property int value: 1
            ButtonGroup.group: reading_select_group
        }

        StyledRadioButton{
            text: "Humidity";
            property int value: 2
            ButtonGroup.group: reading_select_group
        }
    }
}
