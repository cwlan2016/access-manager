import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Window 2.1
import QtQuick.Layouts 1.0
import "qrc:/qml/gui"

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480

    menuBar: MenuBar {
        Menu {
            title: qsTr("Файл")

            MenuItem {
                id: loadDeviceList
                text: qsTr("Загрузить список устройств")
                //onTriggered: Qt.quit();
            }
            MenuItem {
                id: saveDeviceList
                text: qsTr("Сохранить список устройств")
                //onTriggered: Qt.quit();
            }

            MenuItem {
                text: qsTr("Выход")
                onTriggered: Qt.quit();
            }
        }
    }
    
    toolBar: ToolBar {
        RowLayout {
            ToolButton {
                id: b1
                iconSource: "qrc:/add.png"
                width: 48
                height: 60
            }

            ToolButton {
                id : b2
                iconSource: "qrc:/remove.png"
                width: 48
                height: 60
            }
        }
    }


    TabView {
        anchors.fill: parent

        DevicePage {
            title: "hello"
        }
    }

    statusBar: StatusBar {

    }
}
