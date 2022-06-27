#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QVector>
#include "chessboard.h"
#include "qtcpsocket.h"

class ChessBoard;
/**
 * @brief The Player class 玩家类，管理棋子并存储姓名、方位等必要信息
 */
class Player : public QObject {
   public:
    Player(int _color = 1, int _spawn = 1, int _target = 4, unsigned int _flag=2, QString _name = "张三", int _chessNum = 10);
    ~Player();

    /**
     * @brief parentChessBoard 是父棋盘
     */
    ChessBoard* parentChessBoard;

    /**
     * @brief toAlgoPlayer 提取对AI算法有用的信息用于传输
     * @return 见 AlgoPlayer
     */
    AlgoPlayer toAlgoPlayer();

    /**
     * @brief name 玩家的名字
     */
    QString name;

    /**
     * @brief lstMove 上次的移动路径
     */
    QVector<ChessPosition> lstMove;

    /**
     * @brief replay 重播上次移动
     */
    void replay();

    /**
     * @brief chessNum 棋子数
     */
    int chessNum;

    /**
     * @brief color 棋子颜色，或类别序号
     * @note 0 表示提示棋子
     */
    int color;

    /**
     * @brief spawn 起点位置编号
     */
    int spawn;

    /**
     * @brief spawn 终点位置编号
     */
    int target;

    /**
     * @brief flag 00:服务器玩家，本局不移动 01:服务器玩家，本局移动 10:本地玩家，本局不移动 11:本地玩家，本局移动 100:跳过(已结束或犯规)
     */
    unsigned int flag;

    /**
     * @brief chesses 拥有的棋子
     */
    QVector<Marble*> chesses;

    /**
     * @brief getChess 通过坐标获得棋子指针
     * @param p 坐标
     * @return 棋子指针
     */
    Marble* getChess(ChessPosition p);
    Marble* getChess(int x,int y);

    /**
     * @brief checkWin 判断是否获胜
     * @return 是否获胜
     */
    bool checkWin();

    /**
     * @brief addTo 绑定到父棋盘上
     * @param _parentChessBoard 父棋盘
     */
    void addTo(ChessBoard* _parentChessBoard = 0);

    /**
     * @brief setActivated 改变激活状态，改变棋子的悬浮手势
     * @param flag 是否激活
     */
    void setActivated(bool flag);

    /**
     * @brief clear delete自己所有的棋子
     */
    void clear();

    virtual bool isReady() const;
    virtual QTcpSocket* getSocket() const;

    /**
     * @brief agent_algorithm 当前使用的AI算法
     */
    Agent_algorithm *agent_algorithm;
};

#endif  // PLAYER_H
