#ifndef MARBLE_H
#define MARBLE_H
#include <QLabel>
#include "settings.h"
#include "chessboard.h"

class Marble: public QLabel
{
public:
    Marble(ChessBoard *_parentChessBoard=0, int _x=0, int _y=0, int _color=1);
    ChessBoard *parentChessBoard;
    int chessX,chessY,chessColor;
    ChessPostion chessPosition;
    void moveTo(int _x,int _y);
    bool isInEdge();
};

#endif // MARBLE_H
