#include "marble.h"
#include <QPoint>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QString>
#include <stack>
#include "chessboard.h"
#include "player.h"
#include "settings.h"
#include "util.h"
#include "widget.h"

Marble::Marble(Widget* _parentWindow, int _x, int _y, int _color)
    : QLabel(_parentWindow), chessX(_x), chessY(_y), chessColor(_color), chessPosition(_x, _y) {
    connect(this, SIGNAL(clicked()), this, SLOT(On_Clicked()));
    setText(getColorName(chessColor));
    setStyleSheet(getQColor(chessColor));
}

Marble::~Marble() {
    ;
}

void Marble::moveTo(int _x, int _y) {
    // 正常棋子要记 occupiedPst，提示棋子不能记
    if (this->chessColor != color::hint) {
        parentPlayer->parentChessBoard->occupiedPst[chessX + board::indexBoundary][chessY + board::indexBoundary] = false;
        chessX = _x;
        chessY = _y;
        parentPlayer->parentChessBoard->occupiedPst[chessX + board::indexBoundary][chessY + board::indexBoundary] = true;
    }
    chessPosition = ChessPostion(chessX, chessY);
    ChessPostion transformedPosition = boardTransform(chessX, chessY);
    int transformedX = transformedPosition.first, transformedY = transformedPosition.second;
    setGeometry(transformedX - marbleinfo::r, transformedY - marbleinfo::r, 2 * marbleinfo::r, 2 * marbleinfo::r);
}

void Marble::moveTo(ChessPostion pst) {
    moveTo(pst.first, pst.second);
}

void Marble::moveToWithPath(Marble* dest) {
    // 正常棋子要记 occupiedPst，提示棋子不能记
    // 这里的移动是改变棋盘坐标，像素坐标并没有改变，像素坐标会在下面的动画中被慢慢改变
    if (this->chessColor != color::hint) {
        parentPlayer->parentChessBoard->occupiedPst[chessX + board::indexBoundary][chessY + board::indexBoundary] = false;
        chessX = dest->chessX;
        chessY = dest->chessY;
        chessPosition = ChessPostion(chessX, chessY);
        parentPlayer->parentChessBoard->occupiedPst[chessX + board::indexBoundary][chessY + board::indexBoundary] = true;
    }

    // 使用堆结构实现逆推路径 + 顺序播放。值得注意的是，这里路径的记录是全部采用指针的方法，所以在移动结束前不能删除 hint 棋子
    Marble* now = dest;
    std::stack<Marble*> S;
    while (now != this) {
        S.push(now);
        now = now->from;
    }

    // Qt动画是多线程的，如果要串联/并联顺序播放可以用它的 动画组 库。
    // 注意这里移动是真的会慢慢改参数，而不是仅仅显示上，所以创建移动动画后不需要再调用一次改变位置的函数
    // 但这样的问题就是如果你连续创建动画的话你必须提前告诉它上一次移动的终点为起点，而不能让他自己去获取
    // 我不知道 Qt 支不支持线程阻塞，如果支持的话就可以用阻塞来实现，似乎会更优雅。
    // 目前非阻塞会有一个小 Bug 就是在 AI 看来棋子已经移动结束了，所以开托管后人看起来就是棋盘山同时有很多棋子的移动，因为上一次的动画还没播完

    QSequentialAnimationGroup* sequGroup = new QSequentialAnimationGroup;
    QPoint lstPst = this->pos();
    while (!S.empty()) {
        QPropertyAnimation* pPosAnimation = new QPropertyAnimation(this, "pos");
        pPosAnimation->setDuration(500);
        pPosAnimation->setStartValue(lstPst);
        pPosAnimation->setEndValue(S.top()->pos());
        lstPst = S.top()->pos();
        pPosAnimation->setEasingCurve(QEasingCurve::InOutQuad);
        sequGroup->addAnimation(pPosAnimation);
        S.pop();
    }

    // 突然发现一个问题，这个对象会不会自动 delete 自己啊，好像是自动结束后自我销毁的，这个需要确认
    sequGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

bool Marble::isWithinBoundary_() {
    return isWithinBoundary(this->chessPosition);
}

void Marble::addTo(Player* _parentPlayer) {
    parentPlayer = _parentPlayer;
    // occupiedPst 的更新也包含在 moveTo() 里
    moveTo(chessX, chessY);
}

bool Marble::isCollinearWith(const Marble& rhs) const {
    return isCollinear(this->chessPosition, rhs.chessPosition);
}

void Marble::On_Clicked() {
    if (this->parentPlayer->activated) {
        qDebug() << "I'm clicked";
        // 两种棋子对应不同的回调
        if (chessColor == color::hint) {
            this->parentPlayer->parentChessBoard->moveChess(this);
        } else {
            this->parentPlayer->parentChessBoard->chooseChess(this);
        }
    }
}

void Marble::mousePressEvent(QMouseEvent* event) {
    emit clicked();
    Q_UNUSED(event);
    //    QLabel::mousePressEvent(event);
}
