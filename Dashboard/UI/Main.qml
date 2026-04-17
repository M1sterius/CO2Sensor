import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../UI"

Window {
    visible: true
    title: "Air Quality Dashboard"

    width: 1024
    height: 576
    minimumWidth: 1024
    minimumHeight: 576

    Rectangle{
        anchors.fill: parent
        z: -1

        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop { position: 0.0; color: "#4480bc" }
            GradientStop { position: 1.0; color: "#487fb8" }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 4
        spacing: 4

        ButtonsRow {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.06
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.9
            spacing: 4

            DashboardCard {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: 42
            }

            GraphCard {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: 58
            }
        }
    }
}
