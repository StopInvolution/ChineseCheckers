#ifndef MARBLE_H
#define MARBLE_H
#include <QLabel>
#include "clickableqlabel.h"
#include "settings.h"

class Widget;
class ChessBoard;
class Player;
class Marble:public ClickableQLabel
{
    Q_OBJECT
   public:
    Marble(Widget* _parentWindow = 0, int _x = 0, int _y = 0, int _color = 1);
    ~Marble();

    /**
     * @brief parentPlayer 是父玩家
     */
    Player* parentPlayer;

    /**
     * @brief from 记录跳过来的 BFS 父棋子，对于 hint 棋子才有意义，用来获得路径
     */
    Marble* from;

    /**
     * @brief chessX 是棋盘横坐标
     */
    int chessX;

    /**
     * @brief chessY 是棋盘纵坐标
     */
    int chessY;

    /**
     * @brief chessColor 是棋子颜色
     */
    int chessColor;

    /**
     * @brief chessPosition 棋盘坐标(xy)
     */
    ChessPosition chessPosition;

    /**
     * @brief addTo 附加到玩家上，同时修改鼠标手势
     * @param _parentPlayer 所属玩家
     */
    void addTo(Player* _parentPlayer);

    /**
     * @brief moveTo 移动并更新显示
     * @param _x 终点横坐标
     * @param _y 终点纵坐标
     */
    void moveTo(int _x = 0, int _y = 0);

    /**
     * @brief moveTo 移动并更新显示
     * @param pst 终点坐标
     */
    void moveTo(ChessPosition pst);

    /**
     * @brief moveToWithPath 根据路径移动
     * @param dest 终点，可以为空
     * @param path 路径，可以为空，但不能和 dest 都为空
     * @param burn 移动后是否销毁
     * @note 如果有 dest 就认为是利用 from 记录路径，会用堆来获得路径。否则就认为 path 带了完整路径。
     */
    void moveToWithPath(Marble* dest, QVector<ChessPosition>* path=nullptr, bool burn=false);

    /**
     * @brief isWithinBoundary_ 是否在棋盘内
     * @return 是否在棋盘内
     */
    bool isWithinBoundary_();

    /**
     * @brief isCollinearWith 是否和另有一个棋子共线，线是游戏中的线
     * @param rhs 另一个棋子
     * @return 是否共线
     */
    bool isCollinearWith(const Marble& rhs) const;

   public slots:
    void On_Clicked();
};

#endif  // MARBLE_H
