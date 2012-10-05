import QtQuick 1.1


Rectangle {
 width: 300
 height: 400
 color: "#cfc313"

 Text {
     id: text1
     x: 85
     y: 89
     width: 138
     height: 24
     text: qsTr("Text example")
     font.bold: true
     font.pixelSize: 20
 }

 Rectangle {
     id: rectangle1
     x: 72
     y: 152
     width: 113
     height: 77
     color: "#a81b1b"
 }

 Rectangle {
     id: rectangle2
     x: 141
     y: 268
     width: 113
     height: 77
     color: "#1b2ba8"
 }
}



