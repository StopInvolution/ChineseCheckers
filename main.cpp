#include "widget.h"
#include "waiting_room.h"
#include "initwidget.h"
#include "util.h"
#include <QApplication>

int main(int argc, char *argv[])
{
//    qDebug()<<(QString("A").compare(QString("B")));
    qDebug()<<getSpawn("F")<<" "<<getTarget("F");
    QApplication a(argc, argv);
    Widget w;
    initWidget initW;
    WaitingRoom waitR;
    initW.show();
    QObject::connect(&initW,&initWidget::start,&w,&Widget::initChessBoard);
    QObject::connect(&initW,&initWidget::startOnline,&waitR,&WaitingRoom::initWindow);
    return a.exec();
}
