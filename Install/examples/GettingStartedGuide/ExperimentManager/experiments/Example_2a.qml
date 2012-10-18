import QtQuick 1.1

//Here's some comment...

Rectangle
{
    id: rectStimuliArea
    width: 700
    height: 500
    color: "#ffffff" //White background color

    Text
    {
        id: txtFixation
        width: 60
        height: 50
        anchors.horizontalCenter: rectStimuliArea.horizontalCenter //Anchors provide a way to position an item by specifying its relationship with other items.
        anchors.verticalCenter: rectStimuliArea.verticalCenter

        color: "#ff0000" //Red fixation cross
        text: qsTr("+")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.bold: true
        font.pixelSize: 20
    }
}
