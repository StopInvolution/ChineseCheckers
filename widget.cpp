#include "widget.h"
#include "ui_widget.h"
#include "chessboard.h"
#include "util.h"
#include <QPixmap>
#include <QPalette>
#include <QDebug>
#include "windowAddress.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , playerNum(2)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
//    ui->label->setVisible(false);
    setWindowTitle("中国跳棋 By Team9");
    this->setMinimumSize(QSize(800, 625));
    this->setMaximumSize(QSize(800, 625));
    QPixmap bkgnd(":/images/background.png");
    bkgnd = bkgnd.scaled(this->size(),Qt::IgnoreAspectRatio);
    QPalette palatte;
    palatte.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palatte);

    chessBoard = new ChessBoard(this,playerNum);

    btnSetPlayerNum = new QPushButton(this);
    btnSetPlayerNum->setGeometry(30,360,100,30);
    btnSetPlayerNum->setText("SetPlayerNum");
    btnSetPlayerNum->setCursor(Qt::PointingHandCursor);
    connect(this->btnSetPlayerNum,&QPushButton::clicked,this,&Widget::on_btnSetPlayerNum_clicked);

    labelPlayerNum = new QLabel(this);
    labelPlayerNum->setGeometry(30,410,30,30);
    labelPlayerNum->setText("人数:");

    editPlayerNum = new QLineEdit(this);
    editPlayerNum->setGeometry(60,410,70,30);
//    editPlayerNum->setText("2");
    editPlayerNum->setAlignment(Qt::AlignHCenter);
    editPlayerNum->setClearButtonEnabled(true);
    editPlayerNum->setPlaceholderText("1,2,3,4,6");
}

Widget::~Widget()
{
    delete ui;
    delete chessBoard;
}

void Widget::initChessBoard(int kind,int newPlayerNum,QVector<pss>* playerInfo, std::map<QString,bool>* localFlag, NetworkSocket* _socket)
{
    this->setChessBoard(kind,newPlayerNum,playerInfo,localFlag,_socket);
    this->show();
}

void Widget::closeEvent(QCloseEvent *event){
    if(this->chessBoard->initLocalFlag.size()){
        WindowAddress::mainWindow->show();
        this->chessBoard->gameResult="stop";
        if(this->socket){
            //this->socket->bye();
        }
        this->hide();
    }
    else{
        QWidget::closeEvent(event);
    }
}

void Widget::setChessBoard(int kind,int newPlayerNum,QVector<pss>* playerInfo, std::map<QString,bool>* localFlag, NetworkSocket* _socket)
{
    if(this->chessBoard){
        delete this->chessBoard;
        this->chessBoard=nullptr;
    }
    playerNum = newPlayerNum;

    switch(kind){
    case CB::LOCAL:
        chessBoard = new ChessBoard(this,playerNum,playerInfo,localFlag);
        break;
    case CB::CLIENT:
        chessBoard = new SocketChessBoard(this,playerNum,playerInfo,localFlag,_socket);
        break;
    case CB::SERVER:
        chessBoard = new ServerChessBoard(this,playerNum,playerInfo,localFlag);
        break;
    }

    chessBoard->show();
}

void Widget::on_btnSetPlayerNum_clicked()
{
    int t=editPlayerNum->text().toInt();
    if(valid_check(t)){
        QVector<std::pair<QString,QString>> playerInfo;
        std::map<QString,bool> localFlag;
        for(int i=0;i<t;i++){
            QString name="我是"+QString::number(i);
            playerInfo.push_back(std::make_pair(name,getID(board::playerSpawn[t][i])));
            localFlag.insert(std::make_pair(name,true));
        }
        setChessBoard(CB::LOCAL, t,&playerInfo,&localFlag,nullptr);
    }
}
