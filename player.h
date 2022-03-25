#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <vector>
#include "chessboard.h"

class ChessBoard;
class Player : public QObject {
   public:
    Player(int _color = 1, int _spawn = 0, int _target = 3, QString _name = "张三", int _chessNum = 10);
    ~Player();
    ChessBoard* parentChessBoard;

    // 玩家名
    QString name;

    // 棋子数和棋子颜色(泛颜色)，color=color::hint
    int chessNum, color;

    // 棋子的起点和终点
    int spawn, target;

    // 是否是行棋状态
    bool activated;

    // 棋子
    std::vector<Marble*> chesses;

    // 绑定父节点
    void addTo(ChessBoard* _parentChessBoard = 0);

    void setActivated(bool flag);

    // delete chess，主要是为了 hintPlayer 准备的，hintPlayer 经常需要删掉自己所有棋子
    void clear();
};

#endif  // PLAYER_H
