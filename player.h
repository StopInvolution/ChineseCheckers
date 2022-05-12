#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <vector>
#include "chessboard.h"
#include "qtcpsocket.h"

class ChessBoard;
class Player : public QObject {
   public:
    Player(int _color = 1, int _spawn = 0, int _target = 3, unsigned int _flag=2, QString _name = "张三", int _chessNum = 10);
    ~Player();
    ChessBoard* parentChessBoard;

    // 玩家名
    QString name;

    // 棋子数和棋子颜色(泛颜色)，color=color::hint
    int chessNum, color;

    // 棋子的起点和终点
    int spawn, target;

    // 00:服务器玩家，本局不移动 01:服务器玩家，本局移动 10:本地玩家，本局不移动 11:本地玩家，本局移动 100:跳过(已结束或犯规)
    unsigned int flag;

    // 棋子
    std::vector<Marble*> chesses;

    Marble* getChess(ChessPosition p);
    Marble* getChess(int x,int y);
    bool checkWin();

    // 绑定父节点
    void addTo(ChessBoard* _parentChessBoard = 0);

    void setActivated(bool flag);

    // delete chess，主要是为了 hintPlayer 准备的，hintPlayer 经常需要删掉自己所有棋子
    void clear();

    virtual bool isReady() const;
    virtual QTcpSocket* getSocket() const;
};

#endif  // PLAYER_H
