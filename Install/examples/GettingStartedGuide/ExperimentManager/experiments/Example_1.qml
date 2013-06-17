import QtQuick 2.0

//Here's some comment...

Rectangle
{
	width: 600
	height: 800
	color: "#ffff00" //Yellow color

	Text
	{
		id: text1
		x: 85
		y: 89
		width: 138
		height: 24
		text: qsTr("Text example")
		renderType: Text.NativeRendering
		font.bold: true
		font.pixelSize: 20
	}

	Rectangle
	{
		id: rectangle1
		x: 72
		y: 152
		width: 113
		height: 77
		color: "#ff0000"
	}

	Rectangle 
	{
		id: rectangle2
		x: 141
		y: 268
		width: 113
		height: 77
		color: "#0000ff"
	}
}