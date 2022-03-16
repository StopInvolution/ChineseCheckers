#include "chessboard.h"
#include "player.h"
#include "marble.h"
#include "settings.h"


ChessBoard::ChessBoard(QWidget *_parentWindow, int _player_num):parentWindow(_parentWindow),player_num(_player_num),selectChess(0)
{
    for(int i=0;i<player_num;i++){
        player[i] = new Player(board::playerSpawn[player_num][i],board::playerSpawn[player_num][i],board::playerTarget[player_num][i]);
        player[i]->addTo(this);
    }
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
}
