#include "initwidget.h"
#include "ui_initwidget.h"
#include "widget.h"
#include "settings.h"
#include "util.h"
#include <utility>

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
    int t=ui->lineEdit->text().toInt();
    if(valid_check(t)){
        std::vector<std::pair<QString,QString>> playerInfo;
        std::map<QString,bool> localFlag;
        for(int i=0;i<t;i++){
            QString name="我是"+QString::number(i);
            playerInfo.push_back(std::make_pair(name,getID(board::playerSpawn[t][i])));
            localFlag.insert(std::make_pair(name,true));
        }
        emit this->start(t,&playerInfo,&localFlag);
        this->close();
    }
    else ui->WarnLabel->show();
}


