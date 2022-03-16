#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QLabel>


class Player;
class Marble;
class Widget;
class ChessBoard
{
public:
    ChessBoard(Widget *_parentWindow = 0, int _player_num=6);
    ~ChessBoard();
    Widget *parentWindow;
    int player_num,activatedPlayer;
    Marble *selectChess;
    Player *player[6];
    Marble *hint[100];
    QLabel *labelActivated;
    void setActivatedPlayer(int playerId);
};

#endif // CHESSBOARD_H
