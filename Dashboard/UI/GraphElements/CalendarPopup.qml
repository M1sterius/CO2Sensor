import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root

    property var availableDates: []
    property date selectedDate: new Date()
    signal dateSelected(date selectedDate)

    parent: Overlay.overlay
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    width: 320
    height: 400
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    background: Rectangle {
        color: "#1e1e1e"
        radius: 12
        border.color: "#333"
        border.width: 1

        layer.enabled: true
    }

    contentItem: ColumnLayout {
        spacing: 15
        anchors.margins: 10

        RowLayout {
            Layout.fillWidth: true

            Button {
                text: "‹"
                flat: true
                font.pixelSize: 24
                contentItem: Text { text: parent.text; color: "white"; font: parent.font; horizontalAlignment: Text.AlignHCenter }
                onClicked: {
                    if (monthGrid.month === 0) {
                        monthGrid.year--;
                        monthGrid.month = 11;
                    } else {
                        monthGrid.month--;
                    }
                }
            }

            Text {
                Layout.fillWidth: true
                text: Qt.locale().monthName(monthGrid.month) + " " + monthGrid.year
                color: "white"
                font.pixelSize: 18
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Button {
                text: "›"
                flat: true
                font.pixelSize: 24
                contentItem: Text { text: parent.text; color: "white"; font: parent.font; horizontalAlignment: Text.AlignHCenter }
                onClicked: {
                    if (monthGrid.month === 11) {
                        monthGrid.year++;
                        monthGrid.month = 0;
                    } else {
                        monthGrid.month++;
                    }
                }
            }
        }

        DayOfWeekRow {
            locale: monthGrid.locale
            Layout.fillWidth: true
            delegate: Text {
                text: model.shortName
                color: "#99dcf0f5"
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        MonthGrid {
            id: monthGrid
            Layout.fillWidth: true
            Layout.fillHeight: true

            month: new Date().getMonth()
            year: new Date().getFullYear()

            delegate: Item {
                id: dayDelegate
                implicitWidth: 40
                implicitHeight: 40

                readonly property date gridDate: new Date(model.year, model.month, model.day)

                readonly property bool isSelected: {
                    return gridDate.getFullYear() === root.selectedDate.getFullYear() &&
                           gridDate.getMonth() === root.selectedDate.getMonth() &&
                           gridDate.getDate() === root.selectedDate.getDate()
                }

                readonly property bool isAvailable: {
                    return root.availableDates.some(d => {
                        let avail = new Date(d);
                        return avail.getFullYear() === gridDate.getFullYear() &&
                               avail.getMonth() === gridDate.getMonth() &&
                               avail.getDate() === gridDate.getDate();
                    })
                }

                Rectangle {
                    anchors.centerIn: parent
                    width: 34; height: 34
                    radius: 17

                    color: isSelected ? "#00a2ff" : // Full blue if selected
                            (dayMouse.containsMouse && isAvailable ? "#3300a2ff" : "transparent")

                    border.color: model.today ? "#00a2ff" : "transparent"
                    border.width: 1

                    Behavior on color { ColorAnimation { duration: 100 } }
                }

                Text {
                    anchors.centerIn: parent
                    text: model.day
                    font.pixelSize: 14
                    font.bold: model.today

                    color: {
                        if (model.month !== monthGrid.month) return "#333";
                        if (!dayDelegate.isAvailable) return "#444";
                        return "white";
                    }
                }

                MouseArea {
                    id: dayMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    enabled: dayDelegate.isAvailable && model.month === monthGrid.month
                    onClicked: {
                        root.selectedDate = dayDelegate.gridDate
                        root.dateSelected(dayDelegate.gridDate)
                        root.close()
                    }
                }
            }
        }
    }
}
