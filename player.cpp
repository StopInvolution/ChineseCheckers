#include "player.h"
#include "settings.h"
#include "marble.h"
#include "chessboard.h"

Player::Player(int _color, int _spawn, int _target, QString _name):color(_color),spawn(_spawn),target(_target),name(_name)
{
}

Player::~Player()
{
    for(int i=0;i<10;i++){
        delete chess[i];
    }
}

void Player::addTo(ChessBoard *_parentChessBoard)
{
    parentChessBoard = _parentChessBoard;
    for(int i=0;i<10;i++){
        chess[i] = new Marble(parentChessBoard->parentWindow,Board::spawnInint[color][i*2],Board::spawnInint[color][i*2+1],color);
        chess[i]->addTo(parentChessBoard);
    }
}
