#include "player.h"
#include <QString>
#include "chessboard.h"
#include "marble.h"
#include "player.h"
#include "settings.h"

Player::Player(int _color, int _spawn, int _target, unsigned int _flag, QString _name, int _chessNum)
    : name(_name), chessNum(_chessNum), color(_color), spawn(_spawn), target(_target), flag(_flag) {
}

Player::~Player() {
    for (auto obj : this->chesses) {
        delete obj;
    }
}

AlgoPlayer Player::toAlgoPlayer()
{
    AlgoPlayer ret;
    for(int i=0;i<10;i++){
        ret.pst[i]=this->chesses[i]->chessPosition;
    }
    ret.name=this->name;
    ret.target=this->target;
    ret.spawn=this->spawn;
    return ret;
}

Marble *Player::getChess(ChessPosition p)
{
    for(Marble* chess:this->chesses){
        if(chess->chessPosition==p){
            return chess;
        }
    }
    return nullptr;
}

Marble *Player::getChess(int x, int y)
{
    return getChess(ChessPosition(x,y));
}

bool Player::checkWin()
{
    qDebug()<<"checkWin";
    for(Marble* chess:this->chesses){
        bool chessIn=false;
        for(int i=0;i<10;i++){
            if(chess->chessPosition==ChessPosition(board::spawnPst[this->target][i*2],board::spawnPst[this->target][i*2+1])){
                chessIn=true;
                break;
            }
        }
        qDebug()<<chess->chessPosition<<" "<<chessIn;
        if(!chessIn){
            return false;
        }
    }
    return true;
}

void Player::addTo(ChessBoard* _parentChessBoard) {
    parentChessBoard = _parentChessBoard;
    for (int i = 0; i < chessNum; i++) {
        chesses.push_back(new Marble(parentChessBoard->parentWindow, board::spawnPst[color][i * 2], board::spawnPst[color][i * 2 + 1], color));
        chesses.back()->addTo(this);
        if(parentChessBoard->god){
            chesses.back()->setCursor(Qt::PointingHandCursor);
        }
        //chesses.back()->setText(QString::number(i));
    }
}

void Player::setActivated(bool _flag) {
    flag^=(!(flag & _flag));
    if ((this->flag&2) && _flag) {
        if (!this->chesses.empty()) {
            for (auto chess : this->chesses) {
                // 手型
                chess->setCursor(Qt::PointingHandCursor);
            }
        }
    } else {
        if (!this->chesses.empty()) {
            for (auto chess : this->chesses) {
                // 默认箭头型
                chess->setCursor(Qt::ArrowCursor);
            }
        }
    }
}

void Player::clear() {
    for (auto chess : this->chesses) {
        delete chess;
    }
    this->chesses.clear();
    this->setActivated(false);
}

bool Player::isReady() const {assert(0); return 0;}
QTcpSocket* Player::getSocket() const {assert(0); return nullptr;}
