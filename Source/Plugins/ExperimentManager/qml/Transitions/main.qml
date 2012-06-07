import QtQuick 1.1
import MyLibrary 1.0

Item {
    width: 200; height: 300

    Image {
        x: 15
        y: 91
        width: 100
        height: 94
        objectName: "theImage"
    }

    Text {
        objectName: "theText"
        id: text1
        x: 27
        y: 36
        text: qsTr("test")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.bold: true
        font.pixelSize: 23
    }

    Rectangle {
        objectName: "theRectangle"
        id: rectangle1
        x: 100
        y: 19
        width: 79
        height: 58
        color: "#ffffff"
    }

    //here is a QML item that refers to a currentDateTime value
    //that does not exist in the current scope and can be injected
    //by the c++ code
    Text {
        x: 45
        y: 217
        text: currentDateTime
    }

     Text {
         x: 1
         y: 247
         text: applicationData.getCurrentDateTime()

         //Connections
         //{
         //     target: applicationData
         //     onDataChanged: console.log("The application data changed!")
         //}
     }

      ImageViewer {
//          //Connect to the signals, to connect to Qt C++ signals from
//          //within QML, use a signal handler with the on<SignalName> syntax
//          onImageChanged: {
//              console.log("Image changed!")
//              myObject.cppSlot(99999)
//          }
//          onLoadingError: console.log("Image failed to load:", errorMsg)
          objectName: "theImageViewer"
          image: "smile.png"

          signal someOtherSignal(int statusValue)
          Component.onCompleted: {
              someOtherSignal(ImageViewer.Loading)
          }

          onStatusChanged: {
              if (status == ImageViewer.Ready)
                  console.log("Image viewer is ready!")
          }
      }

      Item {
          Connections {
              target: imageViewer2
              onImageChanged: console.log("Image changed!")
          }
      }

      Item {
          objectName: "theFuntionItem"
          function myQmlFunction(msg)
          {
              console.log("Got message:", msg)
              return "some return value"
          }
      }

      Item {//LeftTop
          id: item1
          width: 100; height: 100
          signal qmlSignal(string msg)
          MouseArea
          {
              anchors.fill: parent
              onClicked: {
                  myObject.cppMethod("Hello from QML")
                  myObject.cppSlot(12345)
              }
          }
      }

      Item {//RightBottom
          id: item2
          objectName: "item2"
          width: 100; height: 100
          signal qmlSignal(string msg)
          x: 100
          y: 200

          MouseArea
          {
              anchors.fill: parent
              onClicked: {
                  item2.qmlSignal("Hello from QML2")
                  myObject.cppMethod2("Hello from QML2")
              }
          }
      }

      Item {
          objectName: "propertyitem"
          property int someNumber: 100
      }

      Rectangle {
          x: 121
          y: 91
          width: 79; height: 100
          color: "#bea9a9"

          MouseArea {
              anchors.fill: parent
              onClicked: {
                  applicationData.backgroundColor = "red"
                  console.log("Rectangle.onClicked event ()")
              }
          }
      }
}
