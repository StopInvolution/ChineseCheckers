#include "chessboard.h"
#include "player.h"
#include "marble.h"
#include "settings.h"
#include "widget.h"
#include "util.h"


ChessBoard::ChessBoard(Widget *_parentWindow, int _player_num):parentWindow(_parentWindow),player_num(_player_num),selectChess(0)
{
    for(int i=0;i<player_num;i++){
        player[i] = new Player(board::playerSpawn[player_num][i],board::playerSpawn[player_num][i],board::playerTarget[player_num][i]);
        player[i]->addTo(this);
    }
    labelActivated=new QLabel(this->parentWindow);
    labelActivated->setGeometry(20,10,180,40);
    labelActivated->setFont(QFont("华光中圆_CNKI",16));
    setActivatedPlayer(0);
}

ChessBoard::~ChessBoard()
{
    for(int i=0;i<player_num;i++){
        delete player[i];
    }
}

void ChessBoard::setActivatedPlayer(int playerId)
{
    player[playerId]->setActivated(false);
    activatedPlayer=playerId;
    player[playerId]->setActivated(true);
    labelActivated->setText(QString("当前走棋方为 ")+getColorName(this->player[activatedPlayer]->color));
}
