#ifndef MARBLE_H
#define MARBLE_H
#include <QLabel>
#include "settings.h"

class Widget;
class ChessBoard;
class Player;
class Marble: public QLabel
{
    Q_OBJECT
public:
    Marble(Widget *_parentWindow=0,  int _x=0, int _y=0, int _color=1);
    Player *parentPlayer;
    int chessX,chessY,chessColor;
    ChessPostion chessPosition;
    void moveTo(int _x,int _y);
    bool isInEdge();
    void addTo(Player *_parentPlayer);
    bool isCollinearWith(const Marble &rhs)const{
        int a(chessX),b(chessY),c(rhs.chessX),d(rhs.chessX);
        return a==c || b==d || b-a==d-c;
    }
};

#endif // MARBLE_H
