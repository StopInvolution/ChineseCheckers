#include "marble.h"
#include "settings.h"
#include "util.cpp"
#include <QString>

Marble::Marble(QWidget *parent, int _x, int _y, int _color):QLabel(parent),chessX(_x),chessY(_y),chessColor(_color),chessPosition(_x,_y)
{
    ChessPostion transformedPosition = boardTransform(chessX,chessY);
    int transformedX=transformedPosition.first, transformedY=transformedPosition.second;
    setGeometry(transformedX-MarbleInfo::r,transformedY-MarbleInfo::r,2*MarbleInfo::r,2*MarbleInfo::r);
    setText(getColorName(chessColor));
}

void Marble::moveTo(int _x, int _y)
{
    chessX=_x;
    chessY=_y;
    chessPosition = ChessPostion(chessX,chessY);
}
