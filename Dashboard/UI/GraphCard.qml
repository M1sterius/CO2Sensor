import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts

import "GraphElements"

Rectangle {
    id: graph_card
    radius: 4

    gradient: Gradient {
        orientation: Gradient.Vertical
        GradientStop { position: 0.0; color: "#234783" }
        GradientStop { position: 1.0; color: "#4480bc" }
    }

    ColumnLayout{
        anchors.fill: parent
        spacing: 0

        Text{
            font.pixelSize: 22
            color: "white"
            text: "Measurements History in Graphs"

            Layout.alignment: Qt.AlignHCenter
        }

        RadioButtonsRow{

        }

        ReadingsGraph{

        }

        CalendarPopup{
            id: calendar_popup

            availableDates: ["2026-04-16", "2026-04-17", "2026-04-18"]
            selectedDate: new Date()

            onDateSelected: (date) => {
                backend.onSelectedDateChanged(date)
            }
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 10

            Text{
                font.pixelSize: 14
                leftPadding: 18
                color: "#dcf0f5"
                verticalAlignment: Text.AlignVCenter

                property int daysAvailable: 0
                property int totalReadings: 0
                property string totalSize: "0 Kb"

                text: "Days available: " + daysAvailable + ", Total readings: " + totalReadings +
                      ", Total size: " + totalSize;

                Connections{
                    target: backend

                    function onReadingsAvailableStatusChanged(da, tr, ts) {
                        daysAvailable = da
                        totalReadings = tr
                        totalSize = ts
                    }
                }
            }

            Item{
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            StyledButton{
                text: "Select date"
                font.pixelSize: 14

                onClicked: calendar_popup.open()
            }
        }
    }
}
