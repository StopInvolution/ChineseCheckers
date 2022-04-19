#include "widget.h"
#include "waiting_room.h"
#include "initwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    initWidget initW;
    WaitingRoom waitR;
    initW.show();
    QObject::connect(&initW,&initWidget::start,&w,&Widget::initChessBoard);
    QObject::connect(&initW,&initWidget::startOnline,&waitR,&WaitingRoom::initWindow);
    return a.exec();
}
