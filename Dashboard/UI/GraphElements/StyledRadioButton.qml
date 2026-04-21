import QtQuick
import QtQuick.Controls

RadioButton {
    id: control
    font.pixelSize: 18

    implicitWidth: contentItem.implicitWidth + leftPadding + rightPadding

    property color uncheckedColor: "#666"
    property color checkedColor: "#00a2ff"
    property color textUncheckedColor: "#99dcf0f5"
    property color textCheckedColor: "#ffffff"

    contentItem: Text {
        text: control.text
        font: control.font
        color: control.checked ? textCheckedColor : textUncheckedColor
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
        border.color: control.checked ? checkedColor : uncheckedColor
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

// RadioButton {
//     id: control

//     // --- CONFIGURABLE COLORS ---
//     property color activeColor: "#007acc"      // Saturated blue for 'checked' state
//     property color inactiveColor: "#4d4d4d"    // Dark gray for better visibility on light backgrounds
//     property color textActiveColor: "#000000"  // Black text when selected
//     property color textInactiveColor: "#555555"// Dimmer gray for unselected
//     property color indicatorBg: "#ffffff"     // Solid white background inside the circle

//     font.pixelSize: 18
//     implicitWidth: contentItem.implicitWidth + leftPadding + rightPadding

//     contentItem: Text {
//         text: control.text
//         font: control.font
//         // Toggle between dark active and gray inactive colors
//         color: control.checked ? control.textActiveColor : control.textInactiveColor
//         verticalAlignment: Text.AlignVCenter
//         leftPadding: control.indicator.width + control.spacing

//         Behavior on color { ColorAnimation { duration: 200 } }
//     }

//     indicator: Rectangle {
//         implicitWidth: 22
//         implicitHeight: 22
//         x: control.leftPadding
//         y: parent.height / 2 - height / 2
//         radius: width / 2

//         // 1. IMPROVED VISIBILITY: Added a solid background
//         color: control.indicatorBg

//         // 2. BORDER CONTRAST
//         border.width: control.checked ? 3 : 2
//         border.color: control.checked ? control.activeColor : control.inactiveColor

//         // Inner Dot (The "Checked" indicator)
//         Rectangle {
//             width: 12
//             height: 12
//             anchors.centerIn: parent
//             radius: width / 2
//             color: control.activeColor
//             visible: control.checked

//             // Visual "Pop" animation
//             scale: control.checked ? 1 : 0
//             Behavior on scale {
//                 NumberAnimation {
//                     duration: 200;
//                     easing.type: Easing.OutBack
//                 }
//             }
//         }
//     }
// }
