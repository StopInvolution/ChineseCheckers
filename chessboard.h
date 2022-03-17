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
    int stepNum;

    // 当前行棋方
    int activatedPlayerID;
    Player* activatedPlayer;

    // 选取后待移动的子
    Marble *selectedChess;

    // 已有棋子的位置
    bool occupiedPst[2*board::indexBoundary+1][2*board::indexBoundary+1];

    std::vector<Player*> players;
    Player * hintPlayer;

    // 显示信息
    QLabel *labelInfo;


    void setActivatedPlayer(Player *player=0);
    void setNextActivatedPlayer();

    // 计算并显示 selectedChess 可落子的位置
    void getHint();
    void showHint();

    void moveChess(ChessPostion pst);
    void chooseChess (Marble *chess);
    void nextTurn();
    void randomMove();

    void updateLabelInfo();
};


bool isAnyChessBetween(ChessBoard* chessBoard, ChessPostion u, ChessPostion mid, ChessPostion v);

#endif // CHESSBOARD_H
