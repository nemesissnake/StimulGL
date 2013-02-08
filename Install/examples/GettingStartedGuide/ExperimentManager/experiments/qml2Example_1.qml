import QtQuick 2.0
import QtMultimedia 5.0

Rectangle {
	width: 720
	height: 720
	color: "green"

	Text {
		text: qsTr("Video/Audio/SoundEffect Example, press '1' for video, '2' for audio, '3' for soundeffect.")
		anchors.centerIn: parent
		renderType: Text.NativeRendering
	}
	
	Video {
		id: video
		width : 800
		height : 600
		source: "E:/Movies/Wildlife.wmv" 
	}
	
	Audio {
		id: playMusic
		source: "E:/Music/Yamaha-SY-35-Clarinet-C5.wav"
	}
	
	//SoundEffect {
	//	id: playSound
	//	source: "E:/Music/Yamaha-SY-35-Clarinet-C5.wav"
	//}

	MediaPlayer {
		id: mediaplayer
		source: "E:/Movies/Wildlife.wmv"    
	}

	VideoOutput {
		width : 300
		height : 300
		source: mediaplayer
	}

	MouseArea {
		id: playArea
		anchors.fill: parent
		onPressed: mediaplayer.play();
	}

	Item {
		anchors.fill: parent
		focus: true
		Keys.onPressed: {
			if (event.key == Qt.Key_1) {
				//console.log("1 pressed");
				mediaplayer.play(); 
				event.accepted = true;
			} else if (event.key == Qt.Key_2) {
				playMusic.play(); 
				event.accepted = true;
			} else if (event.key == Qt.Key_3) {
				//playSound.play(); 
				event.accepted = true;
			} else if (event.key == Qt.Key_4) {
				video.play(); 
				event.accepted = true;
			}
		}
	}
}