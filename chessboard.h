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
    int player_num,activatedPlayer;
    Marble *selectChess;
    Player *player[6];
    Marble *hint[100];
    void setActivatedPlayer(int playerId);
};

#endif // CHESSBOARD_H
