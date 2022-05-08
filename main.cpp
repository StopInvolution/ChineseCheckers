#include "widget.h"
#include "waiting_room.h"
#include "mainwindow.h"
#include "initwidget.h"
#include "util.h"
#include "mul_initwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
//    qDebug()<<(QString("A").compare(QString("B")));
    qDebug()<<getSpawn("F")<<" "<<getTarget("F");
    QApplication a(argc, argv);
    MainWindow m;
    Widget w;
    initWidget initW;
    mul_initwidget mul_initW;
    WaitingRoom waitR;
    QObject::connect(&initW,&initWidget::start,&w,&Widget::initChessBoard);
    QObject::connect(&waitR, &WaitingRoom::start, &w, &Widget::initChessBoard);
    QObject::connect(&waitR, &WaitingRoom::start_netini, &w, &Widget::setSocket);
    QObject::connect(&m, &MainWindow::startSingle, &initW, &initWidget::show);
    QObject::connect(&m, &MainWindow::startMultiple, &mul_initW, &mul_initwidget::show);
    QObject::connect(&mul_initW, &mul_initwidget::enterRoom, &waitR, &WaitingRoom::initWindow);
    m.show();
    return a.exec();
}
