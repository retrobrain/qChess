#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QObject>
#include "helpers.h"

enum class PieceType : wchar_t { King = 9812, Queen, Rook, Bishop, Knight, Pawn};
enum class PieceColor : int { White, Black = 6 };

class ChessPiece : public QObject
{
    Q_OBJECT
public:
    explicit ChessPiece(QObject *parent = 0);
    ChessPiece(const PieceType& type, const PieceColor& color);
    bool isMovementValid(Direction direction, int range, bool take);
    PieceType getType() const { return static_cast<PieceType>(static_cast<int>(m_type) + static_cast<int>(m_color)); }
    PieceColor getColor() const { return m_color; }
    void firstMoveMade() { m_bFirstMoveMade = true; }
private:
    bool isPawnMovementLegal(Direction direction, int range, bool take);
    bool isOneTileMove(int range);

private:
    PieceType  m_type;
    PieceColor m_color;
    bool m_bFirstMoveMade;
    std::vector<int> m_vecPossibleDirection;
};

#endif // CHESSPIECE_H
