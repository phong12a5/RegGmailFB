import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0

Window {
    visible: true
    width: 480
    height: root.height
    title: qsTr("Auto register")
    color:  "#fbfbfb"

    Item {
        id: root
        width: parent.width
        height: lsv.height + closeBtn.height + 100

        ListView{
            id: lsv
            model: AppMain.deviceList
            boundsBehavior: Flickable.StopAtBounds
            width: parent.width
            height: contentHeight
            anchors.centerIn: parent
            clip: true
            delegate: Item {
                width: parent.width
                height: 40
                Text {
                    width: 200
                    height: parent.height
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: 15
                    text: qsTr("Device name: " + AppMain.deviceList[index])
                }
            }
        }
    }

    Button{
        id: startBtn
        width: 100
        height: 50
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 100
        font.pixelSize: 15
        text: "Start"
        onReleased: {
            root.opacity = 0.5
            root.enabled = false
            startBtn.enabled = false
            AppMain.startProgram()
        }
    }

    Button{
        id: closeBtn
        width: 100
        height: 50
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 100
        font.pixelSize: 15
        text: "Close"
        onReleased: {
            AppMain.closeProgram()
        }
    }
}
