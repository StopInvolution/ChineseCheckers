#include "setplayernumwindow.h"
#include "ui_setplayernumwindow.h"

SetPlayerNumWindow::SetPlayerNumWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetPlayerNumWindow)
{
    ui->setupUi(this);
}

SetPlayerNumWindow::~SetPlayerNumWindow()
{
    delete ui;
}
