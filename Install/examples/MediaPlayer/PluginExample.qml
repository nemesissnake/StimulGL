import QtQuick 1.0
import StimulGLExtensions  1.0 as MyStimulGLExtensions // import types from the plugin


Rectangle 
{
	id: page
	width: 500; height: 300
	color: "#FFFFFF" //"#0000FF00" //Make it transparant
	
    Column
    {
        x: 56
        y: 23
        width: 311
        height: 39
        spacing: 10
        DebugMode {}//Present in DebugMode.qml!
    }

	Item
	{
		id: mediaPlayerItemID
		objectName: "mediaPlayerItemID"			
		MyStimulGLExtensions.MediaPlayer // this class is defined in C++ (plugin.cpp)
		{
			id: player
		}
	}

	MouseArea
	{
		id: playArea
		anchors.fill: parent
		onPressed: 
		{ 
			player.play() 
		}
	}
	
//	Item
//	{
//		id: keyHandler
//		anchors.fill: parent
//		focus: true
//		Keys.onPressed:
//		{
//            if (event.key === Qt.Key_1)
//			{
//				console.log("Key_1 pressed");
//                //player.addFile("E:\\Projects\\StimulGL\\Install\\examples\\PluginExample\\Sounds\\level2.wav");
//				event.accepted = true;
//			}
//            else if (event.key === Qt.Key_2)
//			{
//				console.log("Key_2 pressed");
//                //player.addFile("E:\\Projects\\StimulGL\\Install\\examples\\PluginExample\\Sounds\\morsecode.mp3");
//				event.accepted = true;
//			}
//            else if (event.key === Qt.Key_3)
//			{
//				console.log("Key_3 pressed");
//                //player.addFile("E:\\Projects\\StimulGL\\Install\\examples\\PluginExample\\Sounds\\WindowsNotify.wav");
//				event.accepted = true;
//			}
//            else if (event.key === Qt.Key_F1)
//            {
//				console.log("Key_F1 pressed");
//				player.play(0);
//				event.accepted = true;
//			}
//            else if (event.key === Qt.Key_F2)
//			{
//				console.log("Key_F2 pressed");
//				player.play(1);
//				event.accepted = true;
//			}
//            else if (event.key === Qt.Key_F3)
//			{
//				console.log("Key_F3 pressed");
//				player.play(2);
//				event.accepted = true;
//			}
//		}
//	}

    Item
    {
        id: functionsID
        objectName: "functions"
        function setSoundFunction(path)
        {
            console.log("setSoundFunction() Got message():", path)//only for debugging writes to stimulGL output file
            var tmpString = new String(path)
            return player.addFile(tmpString);//.toString().substring(tmpString.toString().lastIndexOf("\\")+1))
        }
        function playSoundFunction(index)
        {
            console.log("playSoundFunction() Got message():", index)//only for debugging writes to stimulGL output file
            player.play(index);
        }
    }
}
