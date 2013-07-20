import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
//import DeviceTableModel 1.0

Tab {
    ColumnLayout {
        anchors.margins: 6
        anchors.fill: parent

        Action {
            iconSource: "qrc:/add.png"
            tooltip: qsTr("Добавить")
            id: addDeviceAction
        }

        Action {
            iconSource: "qrc:/edit.png"
            tooltip: qsTr("Изменить")
            id: editDeviceAction
        }

        Action {
            iconSource: "qrc:/remove.png"
            tooltip: qsTr("Удалить")
            id: removeDeviceAction
        }

        RowLayout {
            id: toolsLayout

            anchors.top: parent.top
            anchors.margins: 6


            ToolButton {
                iconSource: "qrc:/add.png"
                //tooltip: qsTr("Добавить")
            }

            ToolButton {
                action: editDeviceAction
            }

            ToolButton {
                action: removeDeviceAction
            }
        }

        TableView {
            id: deviceView

            frameVisible: true

            anchors.top: toolsLayout.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 6

            model: deviceListModel

            TableViewColumn {
                role: "deviceName"
                title: qsTr("Название")
                width: 200
            }

            TableViewColumn {
                role: "deviceModel"
                title: qsTr("Модель")
                width: 80
            }

            TableViewColumn {
                role: "deviceIp"
                title: qsTr("IP")
                width: 100
            }

            TableViewColumn {
                role: "deviceType"
                title: qsTr("Тип устройства")
                width: 120
            }
        }
    }
}
