#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    w.setObjectName("mainWindow");
    w.setStyleSheet("#mainWindow{border-image: url(:/images/background.png);}");
    w.show();
    return a.exec();
}
