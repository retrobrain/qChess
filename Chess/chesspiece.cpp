#include "chesspiece.h"

ChessPiece::ChessPiece(QObject *parent) : QObject(parent)
{
}

ChessPiece::ChessPiece(const PieceType &type, const PieceColor &color)
    :
      QObject(),
      m_type(type),
      m_color(color),
      m_bFirstMoveMade(false)
{
    switch (m_type)
    {
    case PieceType::Knight:
        m_vecPossibleDirection.push_back(DirectionHelpers::NNE);
        m_vecPossibleDirection.push_back(DirectionHelpers::ENE);
        m_vecPossibleDirection.push_back(DirectionHelpers::ESE);
        m_vecPossibleDirection.push_back(DirectionHelpers::SSE);
        m_vecPossibleDirection.push_back(DirectionHelpers::SSW);
        m_vecPossibleDirection.push_back(DirectionHelpers::WSW);
        m_vecPossibleDirection.push_back(DirectionHelpers::WNW);
        m_vecPossibleDirection.push_back(DirectionHelpers::NNW);
        break;
    case PieceType::Rook:
        m_vecPossibleDirection.push_back(DirectionHelpers::E);
        m_vecPossibleDirection.push_back(DirectionHelpers::W);
        m_vecPossibleDirection.push_back(DirectionHelpers::N);
        m_vecPossibleDirection.push_back(DirectionHelpers::S);
        break;
    case PieceType::Queen:
    case PieceType::King:
        m_vecPossibleDirection.push_back(DirectionHelpers::E);
        m_vecPossibleDirection.push_back(DirectionHelpers::W);
    case PieceType::Pawn:
        m_vecPossibleDirection.push_back(DirectionHelpers::N);
        m_vecPossibleDirection.push_back(DirectionHelpers::S);
    case PieceType::Bishop:
        m_vecPossibleDirection.push_back(DirectionHelpers::NW);
        m_vecPossibleDirection.push_back(DirectionHelpers::NE);
        m_vecPossibleDirection.push_back(DirectionHelpers::SW);
        m_vecPossibleDirection.push_back(DirectionHelpers::SE);
        break;
    }
}

bool ChessPiece::isMovementValid(Direction direction, int range, bool take)
{
    for(auto iter : m_vecPossibleDirection)
        if(iter == direction)
        {
            if(m_type == PieceType::Pawn)
                return isPawnMovementLegal(direction, range, take);
            else if(m_type == PieceType::King)
                return isOneTileMove(range);
            else
                return true;
        }
}

bool ChessPiece::isPawnMovementLegal(Direction direction, int range, bool take)
{
    if(!(m_color == PieceColor::White && direction < DirectionHelpers::W
                    || m_color == PieceColor::Black && direction > DirectionHelpers::E))
        return false;

    int iFirstPawnMove = m_bFirstMoveMade ? 1 : 2;

    if(isOneTileMove(range) && range == DirectionHelpers::S || range / iFirstPawnMove == DirectionHelpers::S)
        return !take;
    else if(abs(range) == DirectionHelpers::SW || abs(range) == DirectionHelpers::SE)
        return take;

    return false;
}

bool ChessPiece::isOneTileMove(int range)
{
    return (range <= DirectionHelpers::SE);
}

