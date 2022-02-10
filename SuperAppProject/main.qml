import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

Window {
    id: _mainWindow
    width: 360
    height: 200
    //maximumHeight: height
    //maximumWidth: width
    //minimumHeight: height
    //minimumWidth: width
    visible: true
    color: "#171166"
    title: "Вопрос на милион"

    flags: Qt.Widget

    property bool easyMode: false
    property real stepLength: Math.max(_noButton.width, _noButton.height)
    property int moveDuration: 120

    function randomMove(){
        var angle = Math.random() * Math.PI * 2
        if (easyMode)
            return

        _mainWindow.x += Math.cos(angle) * stepLength
        _mainWindow.y += Math.sin(angle) * stepLength
        if (_mainWindow.y < height)
            _mainWindow.y = height
        if (_mainWindow.x < width)
            _mainWindow.x = width
    }

    Timer {
        id: _moveTimer
        interval: 100
        repeat: true
        onTriggered: _mainWindow.randomMove()
    }

    Behavior on x {
        PropertyAnimation {
            duration: moveDuration
            easing.type: Easing.OutBounce
        }
    }
    Behavior on y {
        PropertyAnimation {
            duration: moveDuration
            easing.type: Easing.OutBounce
        }
    }

    Text {
        id: _question
        color: "#d5d5d5"
        text: "Ты дэбил...?"
        font.bold: true
        font.weight: Font.Medium
        font.family: "Arial"
        font.pointSize: 20
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: parent.height * 0.2
        }
    }

    Timer {
        id: _closeTimer
        interval: 2000
        onTriggered: _mainWindow.close()
    }

    MyButton {
        id: _noButton
        width: 0.35 * parent.width
        height: 0.3 * parent.height
        radius: _yesButton.radius
        text: "Нет"
        hoverColor: _yesButton.hoverColor
        fontColor: _yesButton.fontColor
        font: _yesButton.font
        pressedColor: _yesButton.pressedColor
        defaultColor: _yesButton.defaultColor

        anchors {
            right: parent.right
            bottom: parent.bottom
            rightMargin: 0.1 * parent.width
            bottomMargin: 0.1 * parent.height
        }

        ParallelAnimation {
            id: _winAnimation
            alwaysRunToEnd: true

            PropertyAnimation {
                target: _mainWindow
                properties: "width"
                to: _mainWindow.width * 4
                duration: _closeTimer.interval * 0.6
                easing.type: easing.InOutBounce
            }

            SequentialAnimation {
                ColorAnimation {
                    target: _mainWindow
                    property: "color"
                    to: "red"
                }
                ColorAnimation {
                    target: _mainWindow
                    property: "color"
                    to: "blue"
                }
                ColorAnimation {
                    target: _mainWindow
                    property: "color"
                    to: "green"
                }
                loops: 2
            }
        }
        onClicked: {
            _question.text = "Победаб подеда, поееееа!"
            _winAnimation.start()
            _closeTimer.start()
        }
        onEntered: _moveTimer.start()
        onExited: _moveTimer.stop()
    }


    MyButton {
        id: _yesButton
        visible: true
        color: "#0d084d"
        radius: 0
        text: "Да"
        hoverColor: "#0f095a"
        fontColor: "#dddddd"
        font.bold: true
        font.pointSize: 16
        font.family: "Arial"
        pressedColor: "#160e87"
        defaultColor: "#0d0849"
        width: _noButton.width
        height: _noButton.height

        onClicked: {
            _question.text = "Ну да..."
            text = "Извэнись"
            _noButton.visible = false
            _closeTimer.start()
        }

        anchors {
            left: parent.left
            bottom: parent.bottom
            bottomMargin: _noButton.anchors.bottomMargin
            leftMargin: _noButton.anchors.rightMargin
        }
    }
}


