#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>


class Player;
class Marble;
class ChessBoard
{
public:
    ChessBoard(QWidget *_parentWindow = 0, int _player_num=6);
    ~ChessBoard();
    QWidget *parentWindow;
    int player_num;
    Marble *selectChess;
    Player *player[6];
    Marble *hint[100];
};

#endif // CHESSBOARD_H
