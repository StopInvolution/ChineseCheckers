#include "widget.h"
#include "ui_widget.h"
#include "chessboard.h"
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
    chessBoard = new ChessBoard(this,6);

    btnRandomMove = new QPushButton(this);
    btnRandomMove->setGeometry(30,210,100,30);
    btnRandomMove->setText("RandomMove");
    btnRandomMove->setCursor(Qt::PointingHandCursor);
    connect(this->btnRandomMove,&QPushButton::clicked,this,&Widget::on_btnRandomMove_clicked);

    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,[&](){this->chessBoard->randomMove();});

    btnAutoMv = new QPushButton(this);
    btnAutoMv->setGeometry(30,260,100,30);
    btnAutoMv->setText("AutoMv");
    btnAutoMv->setCursor(Qt::PointingHandCursor);
    connect(this->btnAutoMv,&QPushButton::clicked,this,&Widget::on_btnAutoMv_clicked);

    btnStopAutoMv = new QPushButton(this);
    btnStopAutoMv->setGeometry(30,310,100,30);
    btnStopAutoMv->setText("StopAutoMv");
    btnStopAutoMv->setCursor(Qt::PointingHandCursor);
    connect(this->btnRandomMove,&QPushButton::clicked,this,&Widget::on_btnStopAutoMv_clicked);
}

Widget::~Widget()
{
    delete ui;
    delete chessBoard;
    delete timer;
}

void Widget::paintEvent(QPaintEvent *)
{
    return ;
    QPainter painter(this);
    painter.setPen(QColor(Qt::black));
    painter.setBrush(QBrush(Qt::yellow));
    int x=693,y=190,r=12;
    painter.drawEllipse(249-r,y-r,2*r,2*r);
    painter.drawEllipse(x-r,y-r,2*r,2*r);
    painter.drawEllipse(471-r,312-r,2*r,2*r);
}

void Widget::on_btnRandomMove_clicked()
{
    this->chessBoard->randomMove();
}

void Widget::on_btnAutoMv_clicked()
{
    this->timer->start(200);
}

void Widget::on_btnStopAutoMv_clicked()
{
    this->timer->stop();
}
