#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QPushButton>
#include <sstream>
#include <QString>
#include <player.h>
#include <agent.h>
#include "widget.h"
#include <networkdata.h>
#include <QString>

ClientWidget::ClientWidget(QWidget *parent,ChessBoard* _parentChessBoard) :
    QWidget(parent),
    parentChessBoard(_parentChessBoard),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);
    this->ui->infoEdit->setAlignment(Qt::AlignBottom);
    connect(this->ui->sendButton,&QPushButton::clicked,this,&ClientWidget::onSend);
}

ClientWidget::~ClientWidget()
{
    delete ui;
}

void ClientWidget::send(std::string data)
{
    std::string type;
    std::istringstream iss(data);
    iss>>type;
    if(type=="god"){
        std::string d1;
        if(iss>>d1){
            if(d1=="0") this->parentChessBoard->god=false;
            else{
                this->parentChessBoard->god=true;
            }
        }
        QString info = QString("God mode is ")+QString(this->parentChessBoard->god?"On":"Off")+QString("\n");
        this->ui->infoEdit->insertPlainText(info);
    }
    if(type=="agent"){
        std::string d1,d2;
        if(iss>>d1>>d2){
            Player* player = this->parentChessBoard->getPlayerByName(QString::fromStdString(d1));
            if(!player){
                QString info = QString::fromStdString(d1) + QString(" does not exist")+QString("\n");
                this->ui->infoEdit->insertPlainText(info);
            }
            else{
                Agent_algorithm *func=get_agent_algorithm(QString::fromStdString(d2));
                if(func){
                    player->agent_algorithm=func;
                    QString info = QString("Change ") + QString::fromStdString(d1) + QString("'s agent_algorithm to ") + QString::fromStdString(d2) + QString("\n");
                    this->ui->infoEdit->insertPlainText(info);
                }
                else{
                    QString info = QString::fromStdString(d2) + QString(" does not exist")+QString("\n");
                    this->ui->infoEdit->insertPlainText(info);
                }
            }
        }
    }
    // 成功后会销毁控制台
    if(type=="set"){
        std::string d1;
        if(iss>>d1){
            this->parentChessBoard->parentWindow->editPlayerNum->setText(QString::fromStdString(d1));
            this->parentChessBoard->parentWindow->on_btnSetPlayerNum_clicked();
        }
    }
}

void ClientWidget::onSend()
{
    QString data = this->ui->sendEdit->text();
    send(data.toStdString());

}
