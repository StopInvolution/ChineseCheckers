#include "waiting_room.h"
#include "ui_waiting_room.h"

WaitingRoom::WaitingRoom(QWidget *parent):
    QWidget(parent), ui(new Ui::WaitingRoom)
{
    setWindowTitle("准备房间");
    spinBoxVal = 0; //uninitalized!!

}

void WaitingRoom::initWindow(int playerNum) {
    show();
    int k = ui->spinBox->value();
    qDebug() << k;
    spinBoxVal = playerNum;
}

void WaitingRoom::spinBoxRecheck(int val)
{
    qDebug() << val;
    ui->spinBox->setValue(3);
//    if(val == 5) ui->spinBox->setValue(2^spinBoxVal);  //if it's initally 4 it will turn into 6.The opposite works as well.
//    spinBoxVal = ui->spinBox->value();
}

WaitingRoom::~WaitingRoom()
{
    delete ui;
}

