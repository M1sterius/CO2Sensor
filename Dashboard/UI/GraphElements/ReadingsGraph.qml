import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtGraphs

Item{
    Layout.preferredWidth: graph_card.width * 0.96
    Layout.preferredHeight: graph_card.height * 0.78
    Layout.alignment: Qt.AlignHCenter

    GraphsView {
        id: readings_graph
        anchors.fill: parent
        antialiasing: true

        property double yMin: 0
        property double yMax: 10
        property string yLabel: "Carbon Dioxide"
        property double graphTimestep: 1

        marginLeft: 0
        marginRight: 0
        marginTop: 5
        marginBottom: 5

        Connections {
            target: backend

            function onUpdateGraph(points, y_min, y_max, timestep, y_label) {
                readings_graph.yMin = y_min;
                readings_graph.yMax = y_max;
                readings_graph.graphTimestep = timestep;
                readings_graph.yLabel = y_label;

                line_series.replace(points)
            }
        }

        theme: GraphsTheme {
            backgroundVisible: false
            plotAreaBackgroundVisible: false
            labelTextColor: "#dcf0f5"
            labelFont.pixelSize: 16

            grid.mainColor: "white"
            grid.mainWidth: 1

            grid.subColor: "gray"
            grid.subWidth: 0.0
        }

        axisX: ValueAxis {
            min: 0
            max: 100
            titleText: "Time"
        }

        axisY: ValueAxis {
            min: readings_graph.yMin
            max: readings_graph.yMax
            titleText: readings_graph.yLabel
        }

        LineSeries {
            id: line_series
            name: "test"
            color: "blue"
            width: 2
        }
    }
}
