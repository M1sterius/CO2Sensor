import QtQuick
import QtQuick.Layouts

import "DashboardElements"

Rectangle {
    id: dashboard_card
    radius: 4

    gradient: Gradient {
        orientation: Gradient.Vertical
        GradientStop { position: 0.0; color: "#234783" }
        GradientStop { position: 1.0; color: "#4480bc" }
    }

    ColumnLayout{
        spacing: 0
        anchors.fill: parent

        // Main header
        Text{
            padding: 4
            font.pixelSize: 22
            wrapMode: Text.WordWrap
            text: "Carbon Dioxide Sensor live Dashboard"
            color: "white"

            Layout.alignment: Qt.AlignHCenter
        }

        // Description text
        Text{
            padding: 4
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
            text: "Real time carbon dioxide concentration monitoring represented as PPM - parts per million."
            color: "#dcf0f5"
        }

        RowLayout{
            spacing: 10
            Layout.fillHeight: true
            Layout.fillWidth: true

            CurrentDataCard{
                Layout.preferredWidth: 6
            }

            ConcentrationRangesCard{
                Layout.preferredWidth: 4
            }
        }
    }
}
