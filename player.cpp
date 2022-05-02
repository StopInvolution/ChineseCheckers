#include "player.h"
#include <QString>
#include "chessboard.h"
#include "marble.h"
#include "player.h"
#include "settings.h"

Player::Player(int _color, int _spawn, int _target, QString _name, int _chessNum)
    : name(_name), chessNum(_chessNum), color(_color), spawn(_spawn), target(_target), flag(2) {
}

Player::~Player() {
    for (auto obj : this->chesses) {
        delete obj;
    }
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
    for(Marble* chess:this->chesses){
        bool chessIn=false;
        for(int i=0;i<10;i++){
            if(chess->chessPosition==ChessPosition(board::spawnPst[this->target][i*2],board::spawnPst[this->target][i*2+1])){
                chessIn=true;
                break;
            }
        }
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
