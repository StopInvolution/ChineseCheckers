#include "player.h"
#include "settings.h"
#include "marble.h"
#include "chessboard.h"

Player::Player(int _color, int _spawn, int _target, QString _name):chessNum(10),color(_color),spawn(_spawn),target(_target),name(_name)
{
}

Player::~Player()
{
    for(int i=0;i<chessNum;i++){
        delete chess[i];
    }
}

void Player::addTo(ChessBoard *_parentChessBoard)
{
    parentChessBoard = _parentChessBoard;
    for(int i=0;i<chessNum;i++){
        chess[i] = new Marble(parentChessBoard->parentWindow,board::spawnInint[color][i*2],board::spawnInint[color][i*2+1],color);
        chess[i]->addTo(parentChessBoard);
    }
}

void Player::setActivated(bool flag)
{
    activated=flag;
    if(flag){
        for(int i=0;i<chessNum;i++){
            chess[i]->setCursor(Qt::PointingHandCursor);
        }
    }
    else{
        for(int i=0;i<chessNum;i++){
            chess[i]->setCursor(Qt::ArrowCursor);
        }
    }
}
