import QtQuick 2.0

Rectangle {
    width: 360
    height: 360
    Text {
        text: qsTr("Hello World 1")
        anchors.centerIn: parent
	//renderType: Text.NativeRendering
	smooth: false
	antialiasing: false	    
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }
}