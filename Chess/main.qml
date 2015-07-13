import QtQuick.Controls 1.2
import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Dialogs 1.2
import chess.logics 1.0

ApplicationWindow {
    title: qsTr("qChess")
    visible: true
    minimumWidth:  480
    minimumHeight: 600
    maximumWidth:  480
    maximumHeight: 600

    property int screen_1: 1
    property int screen_2: 2
    property int screen_3: 3

    MessageDialog {
        id: messageDialog
        title: "Error Ocurred"
        text: "Error"
        onAccepted: Qt.quit()
        visible: false
    }

    Connections {
        target: gameController
        onErrorSent: messageDialog.visible = true
    }

    Rectangle{
        id: background
        property alias gameTitle: gameTitle
        anchors.fill: parent
        color: "white"
        Text {
            id: gameTitle
            text: qsTr("qChess")
            font.pointSize: 60
            anchors.bottom: parent.bottom;
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    ChessGrid{
        id: grid
        width: 480; height: 480
    }

    SequentialAnimation{
        id: fadeSequence
        OpacityAnimator{
            id: fadeIn
            target: gameNotification
            from: 0
            to: 1
            duration: 300
        }
        OpacityAnimator{
            id: fadeOut
            target: gameNotification
            from: 1
            to: 0
            duration: 1500
        }
    }

    Text{
        id: gameNotification
        text: qsTr(" ")
        anchors.centerIn: grid
        font.pointSize: 30
        color: "#FFFFF0"
    }

    function showScreen(page){
        scrOneControls.visible = false
        scrTwoControls.visible = false
        scrThreeControls.visible = false
        background.gameTitle.visible = false

        if(page === screen_1)
        {
            scrOneControls.visible = true
            background.gameTitle.visible = true
        }

        else if(page === screen_2)
            scrTwoControls.visible = true

        else
            scrThreeControls.visible = true
    }

    function notificate(str)
    {
        gameNotification.text = qsTr(str)
        fadeSequence.running = true
    }

    Column{
        id: scrOneControls
        anchors.centerIn: grid
        spacing: 4
        property double w: grid.width*0.5
        property double h: minimumHeight - grid.height
        signal startSignal()
        MyButton{ text: qsTr("START"); buttonWidth: parent.w; buttonHeight: parent.h; borderColor: "white";
            onClicked: { gameController.play(); grid.updatePieces(true); showScreen(screen_2) }}
        MyButton{ text: qsTr("LOAD"); buttonWidth: parent.w; buttonHeight: parent.h; borderColor: "white";
            onClicked: { grid.updatePieces(false); showScreen(screen_3) }}
    }

    Row{
        id: scrTwoControls
        visible: false
        anchors.bottom: parent.bottom;
        anchors.horizontalCenter: parent.horizontalCenter
        property double w: grid.width*0.5
        property double h: minimumHeight - grid.height
        MyButton{ text: qsTr("Save"); buttonWidth: parent.w; buttonHeight: parent.h; borderColor: "#F0F0F0";
            onClicked: { notificate(gameController.saveGame())}}
        MyButton{ text: qsTr("Stop"); buttonWidth: parent.w; buttonHeight: parent.h; borderColor: "#F0F0F0";
            onClicked: { gameController.stop(); grid.updatePieces(false); showScreen(screen_1) }}
    }

    Row{
        id: scrThreeControls
        visible: false
        anchors.bottom: parent.bottom;
        anchors.horizontalCenter: parent.horizontalCenter
        property double w: grid.width*0.40
        property double h: minimumHeight - grid.height
        property bool lastFrame: false
        property bool firstFrame: false
        MyButton{ text: qsTr("Start"); buttonWidth: parent.w; buttonHeight: parent.h; borderColor: "#F0F0F0";
            onClicked: { gameController.play(); grid.updatePieces(true); showScreen(screen_2)
            }
        }
        MyButton{ text: qsTr("<<"); buttonWidth: parent.w / 4; buttonHeight: parent.h; borderColor: "#F0F0F0";
            onClicked: { gameController.prevMove(); grid.updatePieces(false) }}
        MyButton{ id: next; text: qsTr(">>"); buttonWidth: parent.w / 4; buttonHeight: parent.h; borderColor: "#F0F0F0";
            onClicked: { if(!gameController.nextMove()); grid.updatePieces(false) }}
        MyButton{ text: qsTr("Load"); buttonWidth: parent.w; buttonHeight: parent.h; borderColor: "#F0F0F0";
            onClicked: { notificate(gameController.loadGame()); grid.updatePieces(false)}}
    }
}

