import QtQuick
import QtQuick.Layouts

RowLayout{
    Layout.fillWidth: true
    Layout.fillHeight: true

    property color range_color;
    property string range_word;
    property string range_text;

    Rectangle{
        width: 30
        height: 30
        radius: width / 2
        color: range_color
    }

    ColumnLayout{
        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: 0

        Text{
            font.pixelSize: 18
            color: "white"
            text: range_word
        }

        Text{
            font.pixelSize: 16
            color: "#dcf0f5"
            text: range_text
        }
    }
}
