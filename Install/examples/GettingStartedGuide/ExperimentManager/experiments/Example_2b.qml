import QtQuick 1.1
import QtMultimediaKit 1.1

//Here's some comment...

Rectangle
{
    id: rectStimuliArea
    width: 700
    height: 500
    color: "#ffffff" //White background color

    Image
    {
        id: imgFruit
        anchors.horizontalCenter: rectStimuliArea.horizontalCenter //Anchors provide a way to position an item by specifying its relationship with other items.
        anchors.verticalCenter: parent.verticalCenter

        fillMode: Image.PreserveAspectFit
        width: 300; height: 300 // This is the area in which the source file is automatically scaled with a preserved aspect fit
        source: "./images/banana.jpg"

        onStatusChanged: if (imgFruit.status == Image.Ready)
        {
            audBeepSound.play()
        }
    }
    Text
    {
        id: txtFruit
        color: "#0000ff" //Blue text color
        text: "banana"
        font.family: "Lucida Sans"
        font.bold: true
        font.pointSize: 32
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: rectStimuliArea.horizontalCenter
        anchors.topMargin: 5
        anchors.top: imgFruit.bottom //here we center the Text below the Image (Including the above margin setting!)
    }
    Audio
    {
        id: audBeepSound
        source: "./audio/audio_1.wav"
    }
}
