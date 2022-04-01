#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QTimer>
#include <QPushButton>
#include <QLineEdit>
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

    // 试验坐标，没有实际用处
    void paintEvent(QPaintEvent *);

    int playerNum;
    ChessBoard *chessBoard;

    QPushButton *btnSetPlayerNum;
    QLineEdit *editPlayerNum;
    QLabel *labelPlayerNum;

    void on_btnSetPlayerNum_clicked();

//private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
