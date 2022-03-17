#include "player.h"
#include "settings.h"
#include "marble.h"
#include "player.h"
#include "chessboard.h"
#include <QString>

Player::Player(int _color, int _spawn, int _target, QString _name,int _chessNum):chessNum(_chessNum),color(_color),spawn(_spawn),target(_target),activated(false),name(_name)
{
}

Player::~Player()
{
    for(auto obj:this->chesses){
        delete obj;
    }
}

void Player::addTo(ChessBoard *_parentChessBoard)
{
    parentChessBoard = _parentChessBoard;
    for(int i=0;i<chessNum;i++){
        chesses.push_back(new Marble(parentChessBoard->parentWindow,board::spawnPst[color][i*2],board::spawnPst[color][i*2+1],color));
        chesses.back()->addTo(this);
//        chesses.back()->setText(QString::number(i));
    }
}

void Player::setActivated(bool flag)
{
    activated=flag;
    if(activated){
        if(!this->chesses.empty()){
            for(auto chess:this->chesses){
                chess->setCursor(Qt::PointingHandCursor);
            }
        }
    }
    else{
        if(!this->chesses.empty()){
            for(auto chess:this->chesses){
                chess->setCursor(Qt::ArrowCursor);
            }
        }
    }
}

void Player::clear()
{
    for(auto chess:this->chesses){
        delete chess;
    }
    this->chesses.clear();
    this->setActivated(false);
}
