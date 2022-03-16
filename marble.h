#ifndef MARBLE_H
#define MARBLE_H
#include <QLabel>
#include <QWidget>
#include "settings.h"

class ChessBoard;
class Marble: public QLabel
{
public:
    Marble(QWidget *_parentWindow=0, int _x=0, int _y=0, int _color=1);
    ChessBoard *parentChessBoard;
    int chessX,chessY,chessColor;
    ChessPostion chessPosition;
    void moveTo(int _x,int _y);
    bool isInEdge();
    void addTo(ChessBoard* parentWindow);
};

#endif // MARBLE_H
