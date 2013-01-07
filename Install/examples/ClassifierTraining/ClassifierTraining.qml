import QtQuick 1.0
import StimulGL_QMLExtensions 1.1 as MyStimulGLExtensions // import types from the plugin
//import StimulGLExtensions 1.1 as MyStimulGLExtensions

Rectangle 
{
	id: page
    width: 768; height: 768
    color: "#00000000" //"#0000FF00" //Make it transparant
	
    Column
    {
        id: column1
        x: 0
        y: 24
        width: 311
        anchors.top: parent.top
        anchors.topMargin: 24
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 705
        anchors.left: parent.left
        anchors.leftMargin: 0
        spacing: 10
        DebugMode{}//Present in DebugMode.qml!
    }

	Item
	{
		id: mediaPlayerItemID
		objectName: "mediaPlayerItemID"			
		MyStimulGLExtensions.MediaPlayer // this class is defined in C++ (plugin.cpp)
		{
			id: player
		}
        //Component.onCompleted:
        //{
        //    player.addFile('E:/Projects/StimulGL/Install/examples/ClassifierTraining/Sounds/talk.wav');
        //    player.addFile('E:/Projects/StimulGL/Install/examples/ClassifierTraining/Sounds/rest.wav');
        //    player.play(0);
        //}
	}

    Text {
        id: fixationID
        objectName: "fixation_cross"
        color: "#ffffff"
        text: qsTr("+")
        //font.underline: true
        //wrapMode: Text.WordWrap
        font.family: "Arial"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        //styleColor: "#ffffff"
        font.bold: true
        font.pixelSize: 25
        //smooth: true
    }

    Item
    {
        id: functionsID
        objectName: "functions"
        function setSoundFunction(path)
        {
          //console.log("setSoundFunction() Got message():", path)//only for debugging writes to stimulGL output file
          var tmpString = new String(path)
          return player.addFile(tmpString);//.toString().substring(tmpString.toString().lastIndexOf("\\")+1))
        }
        function setTextFunction(text)
        {
          console.log("setTextFunction() Got message():", text)//only for debugging writes to stimulGL output file
          var tmpString = new String(text)
          //return player.addFile(tmpString);//.toString().substring(tmpString.toString().lastIndexOf("\\")+1))
          fixationID.text = text;
        }
        function playSoundFunction(index)
        {
          //console.log("playSoundFunction() Got message():", index)//only for debugging writes to stimulGL output file
          return player.play(index);
        }
        function getOutputDevices()
        {
          return player.getOutputDevices();
        }	
        function setOutputDevice(index)
        {
          return player.setOutputDevice(index);
        }	
    }
}
