import QtQuick
import QtQuick.Layouts

ColumnLayout{
    spacing: 12
    Layout.fillWidth: true

    Text{
        font.pixelSize: 20
        color: "white"
        text: "CO2 PPM Ranges"

        Layout.alignment: Qt.AlignHCenter
    }

    RangeCard{
        range_color: "darkgreen";
        range_text: "<450 PPM"
        range_word: "Excellent"
    }

    RangeCard{
        range_color: "green";
        range_text: "450 - 600 PPM"
        range_word: "Good"
    }

    RangeCard{
        range_color: "yellow";
        range_text: "600 - 900 PPM"
        range_word: "Normal"
    }

    RangeCard{
        range_color: "orange";
        range_text: "900 - 1200 PPM"
        range_word: "Poor"
    }

    RangeCard{
        range_color: "red";
        range_text: "1200 - 1500 PPM"
        range_word: "Bad"
    }

    RangeCard{
        range_color: "darkred";
        range_text: "1500 - 2000 PPM"
        range_word: "Awful"
    }

    RangeCard{
        range_color: "black";
        range_text: ">2000 PPM"
        range_word: "Dangerous"
    }

    // Esnures full width and positions header at the top
    Item{
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
}
