#include "widget.h"
#include "initwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    initWidget initW;
    initW.show();
    QObject::connect(&initW,&initWidget::start,&w,&Widget::initChessBoard);
    return a.exec();
}
