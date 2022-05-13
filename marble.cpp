#include "marble.h"
#include <QPoint>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QString>
#include <QPixmap>
#include <QtCore>
#include <stack>
#include "chessboard.h"
#include "player.h"
#include "settings.h"
#include "util.h"
#include "widget.h"

Marble::Marble(Widget* _parentWindow, int _x, int _y, int _color)
    : QLabel(_parentWindow), chessX(_x), chessY(_y), chessColor(_color), chessPosition(_x, _y) {
    connect(this, SIGNAL(clicked()), this, SLOT(On_Clicked()));
    QPixmap pix = QPixmap(QString(":/images/")+getColorName(chessColor)+QString("_marble.png"));
    setPixmap(pix);
    setScaledContents(true);
//    setText(getColorName(chessColor));
//    setStyleSheet(getQColor(chessColor));
}

Marble::~Marble() {
    if(this->chessColor!=color::hint){
        parentPlayer->parentChessBoard->occupiedPst[chessX + board::indexBoundary][chessY + board::indexBoundary] = false;
    }
}

void Marble::moveTo(int _x, int _y) {
    // 正常棋子要记 occupiedPst，提示棋子不能记
    if (this->chessColor != color::hint) {
        parentPlayer->parentChessBoard->occupiedPst[chessX + board::indexBoundary][chessY + board::indexBoundary] = false;
        chessX = _x;
        chessY = _y;
        parentPlayer->parentChessBoard->occupiedPst[chessX + board::indexBoundary][chessY + board::indexBoundary] = true;
    }
    chessPosition = ChessPosition(chessX, chessY);
    ChessPosition transformedPosition = boardTransform(chessX, chessY);
    int transformedX = transformedPosition.first, transformedY = transformedPosition.second;
    setGeometry(transformedX - marbleinfo::r, transformedY - marbleinfo::r, 2 * marbleinfo::r, 2 * marbleinfo::r);
}

void Marble::moveTo(ChessPosition pst) {
    moveTo(pst.first, pst.second);
}

void Marble::moveToWithPath(Marble* dest,std::vector<ChessPosition>* path){
    // 正常棋子要记 occupiedPst，提示棋子不能记
    // 这里的移动是改变棋盘坐标，像素坐标并没有改变，像素坐标会在下面的动画中被慢慢改变
    if (this->chessColor != color::hint) {
        parentPlayer->parentChessBoard->occupiedPst[chessX + board::indexBoundary][chessY + board::indexBoundary] = false;
        // 如果指定路径，那么以路径终点坐标为准
        if(path){
            chessX = path->back().first;
            chessY = path->back().second;
        }
        else{
            chessX = dest->chessX;
            chessY = dest->chessY;
        }
        chessPosition = ChessPosition(chessX, chessY);
        parentPlayer->parentChessBoard->occupiedPst[chessX + board::indexBoundary][chessY + board::indexBoundary] = true;
    }

    std::stack<QPoint> S;
    // 如果没有指定路径，那么按照 hint 棋子追溯路径
    if(!path){
    // 使用堆结构实现逆推路径 + 顺序播放。值得注意的是，这里路径的记录是全部采用指针的方法，所以在移动结束前不能删除 hint 棋子
        Marble* now = dest;
        while (now != this) {
            S.push(now->pos());
            now = now->from;
        }
    }
    else{
        int n=path->size();
        for(int i=n-1;i>=1;i--){
            ChessPosition p=boardTransform((*path)[i]);
            S.push(QPoint(p.first-marbleinfo::r,p.second-marbleinfo::r));
        }
    }

    // Qt动画是多线程的，如果要串联/并联顺序播放可以用它的 动画组 库。
    // 注意这里移动是真的会慢慢改参数，而不是仅仅显示上，所以创建移动动画后不需要再调用一次改变位置的函数
    // 但这样的问题就是如果你连续创建动画的话你必须提前告诉它上一次移动的终点为起点，而不能让他自己去获取
    // 我不知道 Qt 支不支持线程阻塞，如果支持的话就可以用阻塞来实现，似乎会更优雅。
    // 目前非阻塞会有一个小 Bug 就是在 AI 看来棋子已经移动结束了，所以开托管后人看起来就是棋盘上同时有很多棋子的移动，因为上一次的动画还没播完

    QSequentialAnimationGroup* sequGroup = new QSequentialAnimationGroup;
    QPoint lstPst = this->pos();
    while (!S.empty()) {
        QPropertyAnimation* pPosAnimation = new QPropertyAnimation(this, "pos");
        pPosAnimation->setDuration(500);
        pPosAnimation->setStartValue(lstPst);
        pPosAnimation->setEndValue(S.top());
        lstPst = S.top();
        pPosAnimation->setEasingCurve(QEasingCurve::InOutQuad);
        sequGroup->addAnimation(pPosAnimation);
        S.pop();
    }

    // 这个对象在结束后会自动删除自己
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
    // 只有本地当局能点击
    if ((this->parentPlayer->flag==3 && this->parentPlayer->parentChessBoard->serverPermission) || this->parentPlayer->parentChessBoard->god) {
        qDebug() <<this->chessPosition<<" "<<this->chessColor<<" "<< "I'm clicked";
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
