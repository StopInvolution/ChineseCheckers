#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include "settings.h"
#include <vector>


class Player;
class Marble;
class Widget;
class ChessBoard:public QObject
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

    // 是否已被被棋子占据
    bool occupiedPst[2*board::indexBoundary+1][2*board::indexBoundary+1];

    // 玩家
    std::vector<Player*> players;

    // hint 抽象成一个玩家
    Player * hintPlayer;

    // 显示信息
    QLabel *labelInfo;

    // 计算和显示 selectedChess 可落子的位置(hintChess)
    void getHint();
    void showHint();
    void unshowHint();

    // 棋子被点击时的回调函数
    void moveChess(Marble *dest);
    // 落点(hint)被点击时的回调函数
    void chooseChess (Marble *chess);

    // 落子后切换到下一个人
    void nextTurn();
    void setNextActivatedPlayer();
    void setActivatedPlayer(Player *player=0);

    // 随机移动一个棋子
    void randomMove();
    QPushButton *btnRandomMove,*btnAutoMv,*btnStopAutoMv;
    QTimer* timer;
    void on_btnRandomMove_clicked();
    void on_btnAutoMv_clicked();
    void on_btnStopAutoMv_clicked();
    void on_btnSetPlayerNum_clicked();

    // 更新 labelInfo
    void updateLabelInfo();

    // 调用所有对象的show()
    void show();
};

// 棋盘上是否有其他棋子在 u-mid-v 这条线上
bool isAnyChessBetween(ChessBoard* chessBoard, ChessPostion u, ChessPostion mid, ChessPostion v);

#endif // CHESSBOARD_H
