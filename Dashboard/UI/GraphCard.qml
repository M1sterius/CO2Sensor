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

        ReadingSelectRow{

        }

        ReadingsGraph{
            Layout.preferredWidth: graph_card.width * 0.96
            Layout.preferredHeight: graph_card.height * 0.7
            Layout.alignment: Qt.AlignHCenter
        }

        PeriodSelectRow{

        }

        CalendarPopup{
            id: calendar_popup

            // Array of allowed dates in format - 'yyyy-mm-dd'
            availableDates: []
            selectedDate: new Date()

            onDateSelected: (date) => {
                backend.onSelectedDateChanged(date)
            }

            Connections{
                target: backend

                function onSetAvailableDates(dates) {
                    calendar_popup.availableDates = dates
                }
            }
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 10

            Text{
                id: graph_properties
                font.pixelSize: 14
                leftPadding: 18
                color: "#dcf0f5"
                verticalAlignment: Text.AlignVCenter

                property int daysAvailable: 0
                property int totalReadings: 0
                property string totalSize: "0 Kb"

                text: "Days available: " + daysAvailable + ", Total size: " + totalSize;

                Connections{
                    target: backend

                    function onSetGraphProperties(dates, da, ts) {
                        calendar_popup.availableDates = dates
                        graph_properties.daysAvailable = da
                        graph_properties.totalSize = ts
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
