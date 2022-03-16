#include "marble.h"
#include "settings.h"
#include "util.h"
#include "chessboard.h"
#include "widget.h"
#include <QString>

Marble::Marble(Widget* _parentWindow, int _x, int _y, int _color):QLabel(_parentWindow),chessX(_x),chessY(_y),chessColor(_color),chessPosition(_x,_y)
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
    return (chessX<=board::boundary && chessY>=-board::boundary && chessY<=chessX+board::boundary) ||
            (chessX>=-board::boundary && chessY<=board::boundary && chessY>=chessX-board::boundary);
}

void Marble::addTo(Player *_parentPlayer)
{
    parentPlayer=_parentPlayer;
    ChessPostion transformedPosition = boardTransform(chessX,chessY);
    int transformedX=transformedPosition.first, transformedY=transformedPosition.second;
    setGeometry(transformedX-marbleinfo::r,transformedY-marbleinfo::r,2*marbleinfo::r,2*marbleinfo::r);
    setText(getColorName(chessColor));
}
