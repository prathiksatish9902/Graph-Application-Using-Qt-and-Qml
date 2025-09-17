import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import GraphComponents 1.0
GraphItem {
        property alias dataProvider: powerGraph.graphPointsProvider
        id: powerGraph
        anchors.fill: parent
        anchors.margins: 20
        graphPointsProvider: null
}
