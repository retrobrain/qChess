#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QVector>
#include "chesspiece.h"
#include "helpers.h"

enum class GameState { PLAY, PLAYBACK };

struct ChessMove
{
    ChessMove() = default;
    ChessMove(int from, int to) : m_from(from), m_to(to) {}
    int m_from;
    int m_to;
};

struct Tile{
    int _id;
    ChessPiece *_piece;
};

class GameManager : public QObject
{
    Q_OBJECT
public:
    explicit GameManager(QObject *parent = 0);
    ~GameManager();
    Q_INVOKABLE void play();
    Q_INVOKABLE void initialise();
    Q_INVOKABLE void stop();
    Q_INVOKABLE QString getPieceUnicodeSymbol(int index);
    Q_INVOKABLE bool validateMove(int from, int to);
    Q_INVOKABLE bool isPiece(int index);
    Q_INVOKABLE QString saveGame();
    Q_INVOKABLE QString loadGame();
    Q_INVOKABLE bool nextMove();
    Q_INVOKABLE bool prevMove();

signals:

    void errorSent();

protected:
    Q_INVOKABLE void makeMove(int from, int to);

private:
    void clearBoard();
    bool isPieceInBetween(int from, int to, Direction direction);

private:
    PieceColor     m_turnColor;
    QVector<Tile*> m_vecChessBoard;
    QList<ChessMove> m_movesList;
    int m_iCurrentMove = 0;
    GameState m_currentState;
};

#endif // GAMEMANAGER_H
