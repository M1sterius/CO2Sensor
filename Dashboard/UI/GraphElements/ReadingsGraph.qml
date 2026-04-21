import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtGraphs

Item{
    GraphsView {
        id: readings_graph
        anchors.fill: parent
        antialiasing: true

        property double xMin: 12
        property double xMax: 24
        property double yMin: 0
        property double yMax: 10
        property string yLabel: "Carbon Dioxide"

        marginLeft: 0
        marginRight: 0
        marginTop: 5
        marginBottom: 5

        Connections {
            target: backend

            function onUpdateGraph(points, x_min, x_max, y_min, y_max, y_label) {
                readings_graph.xMin = x_min;
                readings_graph.xMax = x_max;
                readings_graph.yMin = y_min;
                readings_graph.yMax = y_max;
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
            min: readings_graph.xMin
            max: readings_graph.xMax
            titleText: "Time (Hh)"
        }

        axisY: ValueAxis {
            min: readings_graph.yMin
            max: readings_graph.yMax
            titleText: readings_graph.yLabel
        }

        LineSeries {
            id: line_series
            name: "test"
            color: "red"
            width: 1.5
        }
    }
}
