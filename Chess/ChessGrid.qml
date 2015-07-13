import QtQuick 2.0

Item {
    id: root
    property int iStartPos: -1

    function getColor(index){
        var evenRow = (Math.floor((index / 8)) % 2 === 0)
        var evenCol = (index % 2 === 0)
        return (evenRow === evenCol) ? "#FFCE9E" : "#D18B47"
    }

    function updatePieces(state)
    {
        for(var i = 0; i < 64; ++i)
        {
            repeater.itemAt(i).pieceSymbol.text = gameController.getPieceUnicodeSymbol(i)
            repeater.itemAt(i).touchArea.visible = state
            repeater.itemAt(i).border.color = repeater.itemAt(i).color
            iStartPos = -1
        }
    }

    Grid {
        id: grid
        columns: 8
        rows: 8
        spacing: 0
        x: 0
        y: 0
        width: root.width
        height: root.height

        Repeater {
            id: repeater
            model: 64
            Rectangle {
                id: chessCell
                width: parent.parent.width / 8
                height: parent.parent.height / 8
                property alias pieceSymbol: symbol
                property alias touchArea: mouseArea
                color: getColor(index)
                border {width: 4; color: chessCell.color}

                Text {
                    id: symbol
                    text: gameController.getPieceUnicodeSymbol(index);
                    anchors.centerIn: parent
                    font.pointSize: 25
                }

                MouseArea{
                    id: mouseArea
                    anchors.fill: parent
                    visible: false
                    hoverEnabled: true
                    property bool isValid: false
                    onEntered: {
                        if(iStartPos !== -1 && iStartPos !== index)
                        {
                            isValid = gameController.validateMove(iStartPos, index)
                            chessCell.border.color = isValid ? "lightgreen" : "red"
                        }
                    }
                    onExited: {
                        if(iStartPos !== index)
                            repeater.itemAt(index).border.color = repeater.itemAt(index).color
                    }
                    onClicked: {
                        if(iStartPos === -1 && gameController.isPiece(index))
                        {
                            iStartPos = index
                            chessCell.border.color = "white"
                        }
                        else if(iStartPos !== -1 && iStartPos !== index && isValid)
                        {
                            gameController.makeMove(iStartPos, index)
                            repeater.itemAt(iStartPos).border.color = repeater.itemAt(iStartPos).color
                            repeater.itemAt(index).pieceSymbol.text = gameController.getPieceUnicodeSymbol(index)
                            repeater.itemAt(iStartPos).pieceSymbol.text = " "
                            iStartPos = -1
                            isValid = false
                        }
                    }
                }
            }
        }
    }
}
