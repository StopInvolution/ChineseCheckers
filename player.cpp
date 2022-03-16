#include "player.h"
#include "settings.h"
#include "marble.h"
#include "player.h"
#include "chessboard.h"
#include <QString>

Player::Player(int _color, int _spawn, int _target, QString _name):chessNum(10),color(_color),spawn(_spawn),target(_target),name(_name)
{
}

Player::~Player()
{
    for(auto obj:this->chess){
        delete obj;
    }
}

void Player::addTo(ChessBoard *_parentChessBoard)
{
    parentChessBoard = _parentChessBoard;
    for(int i=0;i<chessNum;i++){
        chess.push_back(new Marble(parentChessBoard->parentWindow,board::spawnPst[color][i*2],board::spawnPst[color][i*2+1],color));
        chess.back()->addTo(this);
        chess.back()->setText(QString::number(i));
    }
}

void Player::setActivated(bool flag)
{
    activated=flag;
    if(flag){
        for(auto che:this->chess){
            che->setCursor(Qt::PointingHandCursor);
        }
    }
    else{
        for(auto che:this->chess){
            che->setCursor(Qt::ArrowCursor);
        }
    }
}
