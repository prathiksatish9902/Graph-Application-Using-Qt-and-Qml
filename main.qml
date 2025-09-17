import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import "./Components"

Window {
    id: graphWindow
    width: 800
    height: 600
    color: "#1e1e1e"
    visible: true
    title: "EV Power Graph - Random Data"

    GraphWindow {
        id: lineGraph
        anchors.fill: parent
        dataProvider: primaryDataProvider
    }
    Row {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 20
        spacing: 20

        Button {
            text: "Generate New Data"
            onClicked: primaryDataProvider.generateRandomData()

            background: Rectangle {
                color: "#00AEEF"
                radius: 5
            }

            contentItem: Text {
                text: parent.text
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Button {
            text: primaryDataProvider ? "Stop Auto-Gen" : "Start Auto-Gen"
            onClicked: {
                if (primaryDataProvider) {
                    if (primaryDataProvider.isAutoGenerating()) {
                        primaryDataProvider.stopRandomGeneration();
                    } else {
                        primaryDataProvider.startRandomGeneration();
                    }
                }
            }

            background: Rectangle {
                color: "#606060"
                radius: 5
            }

            contentItem: Text {
                text: parent.text
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
