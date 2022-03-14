#include "widget.h"
#include "ui_widget.h"
#include <QPixmap>
#include <QPalette>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->label->setVisible(false);
    this->setMinimumSize(QSize(800, 625));
    this->setMaximumSize(QSize(800, 625));
    QPixmap bkgnd(":/images/background.png");
    bkgnd = bkgnd.scaled(this->size(),Qt::IgnoreAspectRatio);
    QPalette palatte;
    palatte.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palatte);
    chessBoard = new ChessBoard(this,2);
}

Widget::~Widget()
{
    delete ui;
    delete chessBoard;
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QColor(Qt::black));
    //    painter.setBrush(QBrush(Qt::yellow));
    int x=693,y=190,r=12;
    painter.drawEllipse(249-r,y-r,2*r,2*r);
    painter.drawEllipse(x-r,y-r,2*r,2*r);
    painter.drawEllipse(471-r,312-r,2*r,2*r);
}
