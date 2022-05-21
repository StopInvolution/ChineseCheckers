#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QPushButton>
#include <sstream>
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

void ClientWidget::onSend()
{
    QString data = this->ui->sendEdit->text();
    std::string type,d1,d2,d3,d4;
    std::istringstream iss(data.toStdString());
    iss>>type;
    if(type=="god"){
        if(iss>>d1){
            if(d1=="0") this->parentChessBoard->god=false;
            else{
                this->parentChessBoard->god=true;
            }
        }
        QString info = QString("God mode is ")+QString(this->parentChessBoard->god?"On":"Off")+QString("\n");
        this->ui->infoEdit->insertPlainText(info);
    }
}
