import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout{
    spacing: 10
    Layout.alignment: Qt.AlignHCenter

    Row {
        spacing: 15
        anchors.centerIn: parent

        ButtonGroup {
            id: period_select_group

            onClicked: (button) =>{
                backend.onPeriodSelectionRadioButtonClicked(button.value)
            }
        }

        StyledRadioButton{
            text: "1 Hour";
            checked: true;
            property int value: 1

            uncheckedColor: "#424242"
            // checkedColor: "#003e70"
            textUncheckedColor: "#A2B9BD"

            ButtonGroup.group: period_select_group
        }

        StyledRadioButton{
            text: "3 Hours";
            property int value: 3

            uncheckedColor: "#424242"
            // checkedColor: "#003e70"
            textUncheckedColor: "#A2B9BD"

            ButtonGroup.group: period_select_group
        }

        StyledRadioButton{
            text: "9 Hours";
            property int value: 9

            uncheckedColor: "#424242"
            // checkedColor: "#003e70"
            textUncheckedColor: "#A2B9BD"

            ButtonGroup.group: period_select_group
        }

        StyledRadioButton{
            text: "12 Hours";
            property int value: 12

            uncheckedColor: "#424242"
            // checkedColor: "#003e70"
            textUncheckedColor: "#A2B9BD"

            ButtonGroup.group: period_select_group
        }

        StyledRadioButton{
            text: "24 Hours";
            property int value: 24

            uncheckedColor: "#424242"
            // checkedColor: "#003e70"
            textUncheckedColor: "#A2B9BD"

            ButtonGroup.group: period_select_group
        }
    }
}
