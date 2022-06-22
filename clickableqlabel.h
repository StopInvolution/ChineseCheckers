#ifndef CLICKABLEQLABEL_H
#define CLICKABLEQLABEL_H

#include <QObject>
#include <QLabel>
#include "widget.h"
/**
 * @brief 可点击的QLabel，可以发送 clicked() 信号
 */
class ClickableQLabel:public QLabel
{
    Q_OBJECT
public:
    ClickableQLabel(Widget* _parentWindow);
signals:
    void clicked();
protected:
    virtual void mousePressEvent(QMouseEvent* event);//重写mousePressEvent事件
};

#endif // CLICKABLEQLABEL_H
