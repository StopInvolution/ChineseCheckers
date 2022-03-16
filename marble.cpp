#include "marble.h"
#include "settings.h"
#include "util.h"
#include "chessboard.h"
#include "widget.h"
#include "player.h"
#include <QString>

Marble::Marble(Widget* _parentWindow, int _x, int _y, int _color):QLabel(_parentWindow),chessX(_x),chessY(_y),chessColor(_color),chessPosition(_x,_y)
{
    setText(getColorName(chessColor));
}

void Marble::moveTo(int _x, int _y)
{
    // NULL 说明是 hint 类型，只需要更新棋盘位置
    if(parentPlayer!=NULL){
        parentPlayer->parentChessBoard->occupiedPst[chessX+board::indexBoundary][chessY+board::indexBoundary]=false;
        chessX=_x;
        chessY=_y;
        parentPlayer->parentChessBoard->occupiedPst[chessX+board::indexBoundary][chessY+board::indexBoundary]=true;
    }
    chessPosition = ChessPostion(chessX,chessY);
    ChessPostion transformedPosition = boardTransform(chessX,chessY);
    int transformedX=transformedPosition.first, transformedY=transformedPosition.second;
    setGeometry(transformedX-marbleinfo::r,transformedY-marbleinfo::r,2*marbleinfo::r,2*marbleinfo::r);
}

bool Marble::isWithinBoundary_()
{
    return isWithinBoundary(this->chessPosition);
}

void Marble::addTo(Player *_parentPlayer)
{
    parentPlayer=_parentPlayer;
    // occupiedPst 的更新也包含在 moveTo() 里
    moveTo(chessX,chessY);
}

bool Marble::isCollinearWith(const Marble &rhs) const
{
    return isCollinear(this->chessPosition,rhs.chessPosition);
}
