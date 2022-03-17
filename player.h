#ifndef PLAYER_H
#define PLAYER_H


#include "chessboard.h"
#include <QString>
#include <vector>

class ChessBoard;
class Player
{
public:
    Player(int _color=1, int _spawn=0, int _target=3, QString _name="张三",int _chessNum=10);
    ~Player();
    ChessBoard *parentChessBoard;
    int chessNum, color, spawn, target;
    bool activated;
    QString name;
    std::vector<Marble*> chesses;
    void addTo(ChessBoard *_parentChessBoard=0);
    void setActivated(bool flag);
    void clear();
};

#endif // PLAYER_H
