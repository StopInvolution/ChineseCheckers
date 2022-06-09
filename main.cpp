#include "widget.h"
#include "waiting_room.h"
#include "mainwindow.h"
#include "initwidget.h"
#include "util.h"
#include "mul_initwidget.h"
#include "serverwidget.h"
#include "settingswidget.h"
#include <QApplication>
#include "windowAddress.h"

void init() {
    QSettings *settings = new QSettings("../savings/net.ini");
    settings->beginGroup("CLIENT");
    auto tmp = settings->value("IP", "127.0.0.1").toString().split('.');
    for(int i = 0; i<4; ++i)
        Network::ip[i] = tmp[i];
    Network::port = settings->value("port", 8000).toInt();
    settings->endGroup();

    settings->beginGroup("SERVER");
    Network::serverPort = settings->value("port", 8000).toInt();
    settings->endGroup();

    delete settings;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow m;
    WindowAddress::mainWindow=&m;
    ServerWidget* sw = nullptr;
    SettingsWidget *settW = nullptr;
    Widget w;
    initWidget initW;
    mul_initwidget *mul_initW = nullptr;
    WaitingRoom waitR;
    init();
    QObject::connect(&initW,&initWidget::start,&w,&Widget::initChessBoard);
    QObject::connect(&waitR, &WaitingRoom::start, &w, &Widget::initChessBoard);
    QObject::connect(&waitR, &WaitingRoom::__backToTitle, &m, &MainWindow::show);
    QObject::connect(&initW, &initWidget::backToTitle, &m, &MainWindow::show);
    QObject::connect(&m, &MainWindow::startSingle, &initW, &initWidget::show);
    QObject::connect(&m, &MainWindow::startMultiple, [&]() {
        if(mul_initW != nullptr) delete mul_initW;
        mul_initW = new mul_initwidget(); mul_initW->show();
        QObject::connect(mul_initW, &mul_initwidget::enterRoom, &waitR, &WaitingRoom::initWindow);
        QObject::connect(mul_initW, &mul_initwidget::backToTitle, &m, &MainWindow::show);
    });
    QObject::connect(&m, &MainWindow::startServer, [&]() {
        if(sw != nullptr) delete sw;
        sw = new ServerWidget(); sw->show();
    });
    QObject::connect(&m, &MainWindow::startSetting, [&]() {
        if(settW != nullptr) delete settW;
        settW = new SettingsWidget(); settW->init();
    });
    m.show();
    return a.exec();
}
