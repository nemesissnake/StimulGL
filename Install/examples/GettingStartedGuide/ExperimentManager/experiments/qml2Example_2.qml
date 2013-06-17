import QtQuick 2.0

Rectangle 
{
	width: 360
	height: 360
	Text 
	{
		text: qsTr("Hello World 2")
		anchors.centerIn: parent
		renderType: Text.NativeRendering
	}
	MouseArea 
	{
		anchors.fill: parent
		onClicked: 
		{
			Qt.quit();
		}
	}
}