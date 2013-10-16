import QtQuick 2.0

Rectangle 
{
	property alias text: textItem.text

	width: 300; height: 30
	color: "#00ffffff"
	
	Text 
	{
		id: textItem
		anchors.centerIn: parent
		font.pointSize: 14
		color: "#c025a4"
		text: "Block 00, Trial 00, Trigger 00"
		renderType: Text.NativeRendering
	}

	Item 
	{
		id: functionsID
		objectName: "DebugMode_Functions"
		function updateExperimentState(block,trial,trigger) 
		{
			//console.log("Got message(myQmlFunction):", msg)
			textItem.text = "Block " + block + ", Trial " + trial + ", Trigger " + trigger
			return true //"A boolean return value"
		}
	}
}
