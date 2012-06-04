// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0

Rectangle {
    id: page
    width: 500; height: 200
    color: "lightgray"

    Text {
        id: helloText
        text: "Hello world!"
        y: 30
        anchors.horizontalCenter: page.horizontalCenter
        font.pointSize: 24; font.bold: true

        MouseArea { id: mouseArea; anchors.fill: parent }

        states: State {
            name: "down"; when: mouseArea.pressed == true
            PropertyChanges { target: helloText; y: 160; rotation: 180; color: "red" }
        }

        transitions: Transition {
            from: ""; to: "down"; reversible: true
            ParallelAnimation {
                NumberAnimation { properties: "y,rotation"; duration: 500; easing.type: Easing.InOutQuad }
                ColorAnimation { duration: 500 }
            }
        }
    }

    Grid {
        id: colorPicker
        x: 4; anchors.bottom: page.bottom; anchors.bottomMargin: 4
        rows: 2; columns: 3; spacing: 3

        Cell { cellColor: "red"; onClicked: helloText.color = cellColor }
        Cell { cellColor: "green"; onClicked: helloText.color = cellColor }
        Cell { cellColor: "blue"; onClicked: helloText.color = cellColor }
        Cell { cellColor: "yellow"; onClicked: helloText.color = cellColor }
        Cell { cellColor: "steelblue"; onClicked: helloText.color = cellColor }
        Cell { cellColor: "black"; onClicked: helloText.color = cellColor }
    }

    Item {
        id: functionsID
        objectName: "functions"
        function myQmlFunction(msg) {
            console.log("Got message:", msg)
            return helloText.color //"some return value"
        }
    }
}





//Rectangle {
// width: 300
// height: 400
// color: "#cfc313"

// Text {
//     id: text1
//     x: 85
//     y: 89
//     width: 138
//     height: 24
//     text: qsTr("Text example")
//     font.bold: true
//     font.pixelSize: 20
// }

// Rectangle {
//     id: rectangle1
//     x: 72
//     y: 152
//     width: 113
//     height: 77
//     color: "#a81b1b"
// }

// Rectangle {
//     id: rectangle2
//     x: 141
//     y: 268
//     width: 113
//     height: 77
//     color: "#1b2ba8"
// }
//}
