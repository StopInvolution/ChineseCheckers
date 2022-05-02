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
    bool god;

    // 当前行棋方
    int activatedPlayerID;
    Player* activatedPlayer;

    // 选取后待移动的子
    Marble *selectedChess;

    // 是否已被被棋子占据
    bool occupiedPst[2*board::indexBoundary+1][2*board::indexBoundary+1];

    // 玩家
    std::vector<Player*> players;
    std::vector<Player*> winnerRank;

    // hint 抽象成一个玩家
    Player * hintPlayer;

    // 显示信息
    QLabel *labelInfo;

    // 计算和显示 selectedChess 可落子的位置(hintChess)
    void getHint();
    void showHint();
    void unshowHint();

    // 落点(hint)被点击时的回调函数，用于移动选择的普通棋子到该棋子的位置
    void moveChess(Marble *dest);
    // 棋子被点击时的回调函数，用于选择待移动的普通棋子
    void chooseChess (Marble *chess);

    // 落子后，首先判断刚才这一落子是否使得刚才这个人完赛，并做相应处理,
    void nextTurn();
    // 然后切换到下一个人
    void setNextActivatedPlayer();
    void setActivatedPlayer(Player *player=0);

    // moveA2B 自带判断合法，所以原则上不需要调用 checkMove，仅测试用
    // 判断 activatedPlayer 的 p1 位置棋子到 p2 位置是否合法，也就是说必须保证 activatedPlayer 是对的，这一点应该在接受信号时判断，是不是在假装别的人走
    bool checkMove(ChessPosition p1,ChessPosition p2);

    // 将 activatedPlayer 的 p1 位置棋子移动到 p2 位置，合法则移动且返回 true，不合法则不移动且返回 false，同理，这里需要保证是 activatedPlayer 给出的信号
    bool moveA2B(ChessPosition p1,ChessPosition p2);

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

    Marble* getChess(ChessPosition p,int playerID=-1);
    Marble* getChess(int x,int y,int playerID=-1);
};

// 棋盘上是否有其他棋子在 u-mid-v 这条线上
bool isAnyChessBetween(ChessBoard* chessBoard, ChessPosition u, ChessPosition mid, ChessPosition v);

#endif // CHESSBOARD_H
