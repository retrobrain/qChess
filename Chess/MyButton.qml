import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Button{
    id: root
    property int buttonWidth
    property int buttonHeight
    property string borderColor

    style: ButtonStyle {
        id: enabled
        background: Rectangle {
            implicitWidth: buttonWidth
            implicitHeight: buttonHeight
            border.width: control.activeFocus ? 2 : 1
            border.color: borderColor
            radius: 4
            gradient: Gradient {
                GradientStop { position: 0 ; color: control.pressed ? "#F0F0F0" : "#FFFFFF" }
                GradientStop { position: 1 ; color: control.pressed ? "#F0F0F0" : "#FFFFFF" }
            }
        }
        label: Text {
            id: buttonText
            renderType: Text.NativeRendering
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: "dejavu"
            font.pointSize: 20
            text: control.text
        }
    }
}

