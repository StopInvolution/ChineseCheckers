#ifndef MARBLE_H
#define MARBLE_H
#include <QLabel>
#include "settings.h"

class Widget;
class ChessBoard;
class Player;
class Marble: public QLabel
{
    Q_OBJECT
public:
    Marble(Widget *_parentWindow=0,  int _x=0, int _y=0, int _color=1);
    ~Marble();
    Player *parentPlayer;

    // 对于 hint 类型，记录跳过来的路径
    Marble *from;

    int chessX,chessY,chessColor;
    ChessPostion chessPosition;
    void addTo(Player *_parentPlayer);

    // 在棋盘坐标系内移动，同时计算屏幕坐标并刷新显示
    void moveTo(int _x=0,int _y=0);
    void moveTo(ChessPostion pst);
    void moveToWithPath(Marble *dest);

    // 是否在棋盘内
    bool isWithinBoundary_();

    // 棋子是否共线
    bool isCollinearWith(const Marble &rhs)const;

public slots:
    void On_Clicked();

signals:
        void clicked();
protected:
        void mousePressEvent(QMouseEvent* event);
};

#endif // MARBLE_H
