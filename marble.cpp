#include "marble.h"
#include "settings.h"
#include "util.h"
#include "chessboard.h"
#include <QString>

Marble::Marble(QWidget* _parentWindow, int _x, int _y, int _color):QLabel(_parentWindow),chessX(_x),chessY(_y),chessColor(_color),chessPosition(_x,_y)
{
}

void Marble::moveTo(int _x, int _y)
{
    chessX=_x;
    chessY=_y;
    chessPosition = ChessPostion(chessX,chessY);
}

bool Marble::isInEdge()
{
    return (chessX<=Board::boundary && chessY>=-Board::boundary && chessY<=chessX+Board::boundary) ||
            (chessX>=-Board::boundary && chessY<=Board::boundary && chessY>=chessX-Board::boundary);
}

void Marble::addTo(ChessBoard* _parentChessBoard)
{
    ChessPostion transformedPosition = boardTransform(chessX,chessY);
    int transformedX=transformedPosition.first, transformedY=transformedPosition.second;
    parentChessBoard = _parentChessBoard;
    setGeometry(transformedX-MarbleInfo::r,transformedY-MarbleInfo::r,2*MarbleInfo::r,2*MarbleInfo::r);
    setText(getColorName(chessColor));
    setCursor(Qt::PointingHandCursor);
}
