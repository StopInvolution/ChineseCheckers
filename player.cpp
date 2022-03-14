#include "player.h"
#include "settings.h"

Player::Player(ChessBoard *_parentChessBoard, int _color, int _spawn, int _target, QString _name):parentChessBoard(_parentChessBoard), color(_color),spawn(_spawn),target(_target),name(_name)
{
    for(int i=0;i<10;i++){
        chess[i] = new Marble(parentChessBoard,Board::spawnInint[color][i*2],Board::spawnInint[color][i*2+1],color);
    }
}

Player::~Player()
{
    for(int i=0;i<10;i++){
        delete chess[i];
    }
}
