#include "chessboard.h"
#include "player.h"
#include "marble.h"


ChessBoard::ChessBoard(QWidget *_parentWindow, int _player_num):parentWindow(_parentWindow),player_num(_player_num),selectChess(0)
{
    for(int i=0;i<player_num;i++){
        player[i] = new Player(Board::playerSpawn[player_num][i],Board::playerSpawn[player_num][i],Board::playerTarget[player_num][i]);
        player[i]->addTo(this);
    }
}

ChessBoard::~ChessBoard()
{
    for(int i=0;i<player_num;i++){
        delete player[i];
    }
}
