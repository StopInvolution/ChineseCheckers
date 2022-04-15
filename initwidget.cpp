#include "initwidget.h"
#include "ui_initwidget.h"
#include "widget.h"

initWidget::initWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::initWidget)
{
    ui->setupUi(this);
    ui->label->setVisible(false);
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

void initWidget::on_pushButton_clicked()
{
    if(ui->lineEdit->text().toInt()){
        emit this->start(ui->lineEdit->text().toInt());
    }
    this->close();
}

