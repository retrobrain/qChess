#include "gamemanager.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QDataStream>

namespace
{
const int TOTAL_TILES = 64;
const int BLACK_PAWN_ROW_MIN = 7;
const int BLACK_PAWN_ROW_MAX = 16;
const int WHITE_PAWN_ROW_MIN = 47;
const int WHITE_PAWN_ROW_MAX = 56;
}

QDataStream& operator <<(QDataStream &out, const ChessMove &obj)
{
    out << obj.m_from;
    out << obj.m_to;
    return out;
}

QDataStream& operator >>(QDataStream &out, ChessMove &obj)
{
    out >> obj.m_from;
    out >> obj.m_to;
    return out;
}

GameManager::GameManager(QObject *parent) : QObject(parent)
{
}

GameManager::~GameManager()
{
    clearBoard();
}

void GameManager::play()
{
    initialise();
    m_movesList.clear();
    m_currentState = GameState::PLAY;
}

void GameManager::initialise()
{
    m_vecChessBoard.clear();

    try
    {
        for(int index = NULL; index < TOTAL_TILES; ++index)
        {
            Tile* tile = new Tile;
            tile->_id = index;
            tile->_piece = Q_NULLPTR;

            if(index > BLACK_PAWN_ROW_MIN && index < BLACK_PAWN_ROW_MAX)
                tile->_piece = new ChessPiece(PieceType::Pawn, PieceColor::Black);
            else if(index > WHITE_PAWN_ROW_MIN  && index < WHITE_PAWN_ROW_MAX)
                tile->_piece = new ChessPiece(PieceType::Pawn, PieceColor::White);

            m_vecChessBoard.push_back(tile);
        }

        m_vecChessBoard.at(0)->_piece = new ChessPiece(PieceType::Rook, PieceColor::Black);
        m_vecChessBoard.at(7)->_piece = new ChessPiece(PieceType::Rook, PieceColor::Black);
        m_vecChessBoard.at(56)->_piece = new ChessPiece(PieceType::Rook, PieceColor::White);
        m_vecChessBoard.at(63)->_piece = new ChessPiece(PieceType::Rook, PieceColor::White);

        m_vecChessBoard.at(1)->_piece = new ChessPiece(PieceType::Knight, PieceColor::Black);
        m_vecChessBoard.at(6)->_piece = new ChessPiece(PieceType::Knight, PieceColor::Black);
        m_vecChessBoard.at(57)->_piece = new ChessPiece(PieceType::Knight, PieceColor::White);
        m_vecChessBoard.at(62)->_piece = new ChessPiece(PieceType::Knight, PieceColor::White);

        m_vecChessBoard.at(2)->_piece = new ChessPiece(PieceType::Bishop, PieceColor::Black);
        m_vecChessBoard.at(5)->_piece = new ChessPiece(PieceType::Bishop, PieceColor::Black);
        m_vecChessBoard.at(58)->_piece = new ChessPiece(PieceType::Bishop, PieceColor::White);
        m_vecChessBoard.at(61)->_piece = new ChessPiece(PieceType::Bishop, PieceColor::White);

        m_vecChessBoard.at(3)->_piece = new ChessPiece(PieceType::Queen, PieceColor::Black);
        m_vecChessBoard.at(4)->_piece = new ChessPiece(PieceType::King, PieceColor::Black);
        m_vecChessBoard.at(59)->_piece = new ChessPiece(PieceType::Queen, PieceColor::White);
        m_vecChessBoard.at(60)->_piece = new ChessPiece(PieceType::King, PieceColor::White);
    }
    catch(...)
    {
        emit errorSent();
    }

    m_turnColor = PieceColor::White;
}

void GameManager::stop()
{
    clearBoard();
}

QString GameManager::getPieceUnicodeSymbol(int index)
{
    if(m_vecChessBoard.isEmpty())
        return QString(' ');

    auto pPiece = m_vecChessBoard.at(index)->_piece;
    if(pPiece == Q_NULLPTR)
        return QString(' ');

    PieceType type = m_vecChessBoard.at(index)->_piece->getType();
    return  QString(QChar(static_cast<int>(type)));
}

bool GameManager::validateMove(int from, int to)
{
    ChessPiece* pPieceToMove;
    ChessPiece* pPieceToTake;
    Tile* pFromTile;
    Tile* pDestinationTile;

    bool bIsValidDirForPiece = false;
    bool bIsIntersecting     = false;
    bool bIsTake             = false;

    pFromTile = m_vecChessBoard.at(from);
    pPieceToMove = pFromTile->_piece;
    if(pPieceToMove == Q_NULLPTR)
        return false;

    pDestinationTile = m_vecChessBoard.at(to);
    bIsTake = (pPieceToTake = pDestinationTile->_piece);

    if(bIsTake)
        if(pPieceToTake->getColor() == pPieceToMove->getColor())
            return false;

    Direction dir = DirectionHelpers::getDirection(from, to);

    bIsIntersecting = isPieceInBetween(from, to, dir);
    bIsValidDirForPiece = pPieceToMove->isMovementValid(dir, abs(from - to), bIsTake);

    return bIsValidDirForPiece && !bIsIntersecting;
}

void GameManager::makeMove(int from, int to)
{
    ChessPiece* pPieceToMove;
    Tile* pFromTile;
    Tile* pDestinationTile;

    pFromTile = m_vecChessBoard.at(from);
    pPieceToMove = pFromTile->_piece;
    pDestinationTile = m_vecChessBoard.at(to);

    if(pPieceToMove->getType() == PieceType::Pawn)
        pPieceToMove->firstMoveMade();

    pDestinationTile->_piece = pPieceToMove;
    pFromTile->_piece = Q_NULLPTR;

    m_turnColor = (m_turnColor == PieceColor::White) ? PieceColor::Black : PieceColor::White;
    if(m_currentState == GameState::PLAY)
    {
        m_iCurrentMove++;
        m_movesList.push_back(ChessMove(from, to));
    }
}

bool GameManager::isPiece(int index)
{
    auto pPiece = m_vecChessBoard.at(index)->_piece;
    if(pPiece == Q_NULLPTR)
        return false;

    return pPiece->getColor() == m_turnColor ? true : false;
}

QString GameManager::saveGame()
{
    if(m_movesList.size() == 0)
        return QString("Nothing to save!");

    QFile file("data.chess");
    if(!file.open(QIODevice::WriteOnly)) {
        return "Unable to save!";
    }

    QDataStream stream( &file );
    stream.setVersion( QDataStream::Qt_5_4 );
    stream << m_movesList;

    file.close();

    return QString("Game Saved!");
}

QString GameManager::loadGame()
{
    QFile file("data.chess");
    if(!file.open( QIODevice::ReadOnly)) {
        return QString("Unable to load!");
    }

    initialise();
    QDataStream stream(&file);
    stream.setVersion( QDataStream::Qt_5_0);
    stream >> m_movesList;
    file.close();

    m_iCurrentMove = m_movesList.size();

    if(m_iCurrentMove == 0)
        return QString("Nothing to load!");

    m_currentState = GameState::PLAYBACK;
    for(auto move : m_movesList)
    {
        makeMove(move.m_from, move.m_to);
    }

    return QString("Game Loaded!");
}

bool GameManager::nextMove()
{
    if(m_iCurrentMove == m_movesList.size())
        return false;

    initialise();
    m_iCurrentMove++;
    for(int i = NULL; i < m_iCurrentMove; ++i)
    {
        ChessMove move = m_movesList.at(i);
        makeMove(move.m_from, move.m_to);
    }

    return true;
}

bool GameManager::prevMove()
{
    if(m_iCurrentMove == 0)
        return false;

    initialise();
    m_iCurrentMove--;
    for(int i = NULL; i < m_iCurrentMove; ++i)
    {
        ChessMove move = m_movesList.at(i);
        makeMove(move.m_from, move.m_to);
    }

    return true;
}

void GameManager::clearBoard()
{
    if(!m_vecChessBoard.isEmpty())
        for(int i = NULL; i < TOTAL_TILES; ++i)
        {
            delete m_vecChessBoard.at(i)->_piece;
            m_vecChessBoard.at(i)->_piece = Q_NULLPTR;
            delete m_vecChessBoard.at(i);
        }
    m_vecChessBoard.clear();
}

bool GameManager::isPieceInBetween(int from, int to, Direction direction)
{
    for(int iStart = from + direction; iStart != to; iStart += direction)
        if(m_vecChessBoard.at(iStart)->_piece != Q_NULLPTR)
            return true;

    return false;
}

