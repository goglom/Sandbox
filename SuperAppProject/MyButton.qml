import QtQuick 2.12
import QtGraphicalEffects 1.0

Rectangle {
    property string name: "MyButton"
    property alias text: _txt.text
    property alias font: _txt.font
    property alias fontColor: _txt.color

    signal entered
    signal exited
    signal clicked
    signal pressed
    signal released

    visible: true
    color: defaultColor
    property color defaultColor: "#cacaca"
    property color pressedColor: "#808080"
    property color hoverColor: "#ffffff"


    DropShadow {
        radius: 8
        horizontalOffset: 3.9

    }

    Text {
        id: _txt
        text: "Button"
        anchors.centerIn: parent
    }

    MouseArea {
        id: _mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onEntered: {
            parent.color = parent.hoverColor
            parent.entered()
        }
        onExited: {
            parent.color = parent.defaultColor
            parent.exited()
        }
        onClicked: parent.clicked()
        onPressed: {
            parent.color = parent.pressedColor
            parent.pressed()
        }
        onReleased: {
            parent.color = parent.defaultColor
            parent.released()
        }
    }

}



/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.75;height:480;width:640}
}
##^##*/
