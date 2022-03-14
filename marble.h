#ifndef MARBLE_H
#define MARBLE_H
#include "color.h"
#include <QLabel>
#include "settings.h"

class Marble: public QLabel
{
public:
    Marble(QWidget *parent = 0, int _x=0, int _y=0, int _color=1);
    int chessX,chessY,chessColor;
    ChessPostion chessPosition;
    void moveTo(int _x,int _y);
};

#endif // MARBLE_H
