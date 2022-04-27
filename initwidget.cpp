#include "initwidget.h"
#include "ui_initwidget.h"
#include "widget.h"

initWidget::initWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::initWidget)
{
    ui->setupUi(this);
    ui->label->setVisible(false);
    ui->WarnLabel->hide();
    QPixmap bkgnd(":/images/init_background.jpeg");
    bkgnd = bkgnd.scaled(this->size(),Qt::IgnoreAspectRatio);
    QPalette palatte;
    palatte.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palatte);
}

initWidget::~initWidget()
{
    delete ui;
}

bool initWidget::valid_check(int num)
{
    switch(num) {
        case 2:
        case 3:
        case 4:
        case 6:
            return true;
    }
    return false;
}

void initWidget::on_pushButton_clicked()
{
    if(valid_check(ui->lineEdit->text().toInt())){
        emit this->start(ui->lineEdit->text().toInt());
        this->close();
    }
    else ui->WarnLabel->show();
}

void initWidget::on_onlinepushButton_clicked()
{
    if(valid_check(ui->lineEdit->text().toInt())){
        emit this->startOnline(ui->lineEdit->text().toInt());
        this->close();
    }
    else ui->WarnLabel->show();
}
