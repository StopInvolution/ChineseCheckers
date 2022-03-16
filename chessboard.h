#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QLabel>
#include "settings.h"
#include <vector>


class Player;
class Marble;
class Widget;
class ChessBoard
{
public:
    ChessBoard(Widget *_parentWindow = 0, int _player_num=6);
    ~ChessBoard();
    Widget *parentWindow;

    int playerNum;

    // 当前行棋方
    Player* activatedPlayer;

    // 选取后待移动的子
    Marble *selectedChess;

    // 已有棋子的位置
    bool occupiedPst[2*board::indexBoundary+1][2*board::indexBoundary+1];

    std::vector<Player*> player;
    std::vector<Marble*> hint;

    // 显示当前行棋方
    QLabel *labelActivated;


    void setActivatedPlayer(Player *player=0);

    // 计算并显示 selectedChess 可落子的位置
    void getHint();
};


bool isAnyChessBetween(ChessBoard* chessBoard, ChessPostion u, ChessPostion v);

#endif // CHESSBOARD_H
