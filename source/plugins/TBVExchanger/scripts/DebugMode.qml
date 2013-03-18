import QtQuick 2.0

Rectangle 
{
	id: debugModeFrame
	property alias text: textItem.text
	width: 300; height: 30
	color: "#00ffffff"
	visible: false

	Text 
	{
		id: textItem
		anchors.centerIn: parent
		font.pointSize: 14
		color: "#c025a4"
		text: "Loop -1, Block 00, Trial 00, Trigger 00"	
	}

	Item 
	{
		id: functionsID
		objectName: "DebugMode_Functions"
		function updateExperimentState(block,trial,trigger,loop) 
		{
			//console.log("Got message(myQmlFunction):", msg)
			textItem.text = "Loop " + loop + ", Block " + block + ", Trial " + trial + ", Trigger " + trigger
			return true //"some return value"
		}
		function enableDebugMode()
		{
			debugModeFrame.visible = true;
			return true
		}
	}
}
