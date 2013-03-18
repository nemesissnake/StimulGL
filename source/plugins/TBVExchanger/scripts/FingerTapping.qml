import QtQuick 2.0

Rectangle
{
	id: page
	objectName: "rootObjectName"	
	color: "black" //Make it transparant

	Text
	{
		id: someText
		anchors.centerIn: parent
		text: "+"
		font.bold: true
		font.family: "Helvetica"
		font.pointSize: 26
		color: "white"
	}

	Item
	{
		id: functionsID
		objectName: "functions"
		function setStimuliFunction(textString)
		{
			var tmpString = new String(textString)
			if(tmpString.match("fixation"))
			{
				image1.visible = false;
				//image1.source = "./images/"
				someText.text = "+";
			}
			else if(tmpString.match("lefthand"))
			{
				image1.visible = true;
				image1.source = "./images/LeftHand.png"
				someText.text = "Left Hand";
			}
			else if(tmpString.match("righthand"))
			{
				image1.visible = true;
				image1.source = "./images/RightHand.png"
				someText.text = "Right Hand";
			}				
			return true
		}		
	}

	Column
	{
		x: 0
		y: 0
		width: 311
		height: 39
		spacing: 10
		DebugMode {}//Present in DebugMode.qml!
	}

	Image
	{
		id: image1
		x: 56
		y: 82
//		anchors.horizontalCenter: parent.horizontalCenter
//		anchors.verticalCenter: parent.verticalCenter
//		fillMode: Image.PreserveAspectFit
		//        sourceSize.width: 459
		//        sourceSize.height: 373
		//        smooth: true
		//        fillMode: Image.PreserveAspectFit
		//        asynchronous: true
		source: ""
	}
}
