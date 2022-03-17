#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QTimer>
#include <QPushButton>
#include "marble.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class ChessBoard;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void paintEvent(QPaintEvent *);
    ChessBoard *chessBoard;
    QPushButton *btnRandomMove,*btnAutoMv,*btnStopAutoMv;
//private:
    Ui::Widget *ui;
    QTimer* timer;
    void on_btnRandomMove_clicked();
    void on_btnAutoMv_clicked();
    void on_btnStopAutoMv_clicked();
};
#endif // WIDGET_H
