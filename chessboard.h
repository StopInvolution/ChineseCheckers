#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include "marble.h"
#include "player.h"

class ChessBoard
{
public:
    ChessBoard(QWidget *_parentWindow = 0, int _player_num=6);
    QWidget *parentWindow;
    int player_num;
    Marble *selectChess;
    Player *player[6];
    Marble *hint[100];
};

#endif // CHESSBOARD_H
