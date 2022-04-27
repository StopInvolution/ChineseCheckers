#include "waiting_room.h"
#include "ui_waiting_room.h"

WaitingRoom::WaitingRoom(QWidget *parent):
    QWidget(parent), ui(new Ui::WaitingRoom)
{
    setWindowTitle("准备房间");
    ui->setupUi(this);
    spinBoxVal = 0; //uninitalized!!
    connect(this->ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxRecheck(int)));
}

void WaitingRoom::initWindow(int playerNum) {
    spinBoxVal = playerNum;
    ui->spinBox->setValue(playerNum);
    show();
}

void WaitingRoom::spinBoxRecheck(int val)
{
    auto target = ui->spinBox;
    if(val == 5) {
        target->blockSignals(1);
        target->setValue(spinBoxVal^2);
        target->blockSignals(0);
    }
    spinBoxVal = target->value();
}

WaitingRoom::~WaitingRoom()
{
    delete ui;
}

