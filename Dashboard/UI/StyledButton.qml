import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Button {
    id: btn
    Layout.fillHeight: true
    Layout.preferredHeight: 40
    hoverEnabled: true

    contentItem: Text {
        text: btn.text
        font: btn.font
        color: "#0f0f0f"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        opacity: btn.hovered ? 1.0 : 0.9
        Behavior on opacity { NumberAnimation { duration: 150 } }
    }

    background: Rectangle {
        implicitWidth: 120
        radius: 4

        color: btn.pressed ? "#9fbad6" :
               btn.hovered ? "#b7cde3" : "#c5d7e8"

        border.color: btn.hovered ? "#33b5ff" : "transparent"
        border.width: 1

        Behavior on color { ColorAnimation { duration: 150 } }

        opacity: btn.enabled ? 1.0 : 0.5
    }
}
