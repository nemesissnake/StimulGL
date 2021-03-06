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
			textItem.text = "Block " + block + ", Trial " + trial + ", Trigger " + trigger;
			return true;
		}
		function updateExperimentState_incLoop(block,trial,trigger,loop) 
		{
			textItem.text = "Block " + block + ", Trial " + trial + ", Trigger " + trigger + ", Loop " + loop;
			return true;
		}		
		function setVisibility(bVisible) 
		{
			textItem.visible = bVisible;
			return true;
		}		
	}
}
