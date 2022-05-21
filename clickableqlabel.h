#ifndef CLICKABLEQLABEL_H
#define CLICKABLEQLABEL_H

#include <QObject>
#include <QLabel>
#include "widget.h"

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
