#include "mul_initwidget.h"
#include "ui_mul_initwidget.h"

const QString mul_initwidget::IP = "127.0.0.1";

mul_initwidget::mul_initwidget(QWidget *parent) :
    QWidget(parent),
    isConnected(false),
    ui(new Ui::mul_initwidget),
    username("Team_Why")
{
    ui->setupUi(this);
    this->socket = new NetworkSocket(new QTcpSocket(), this);
    connect(socket, &NetworkSocket::receive, this, &mul_initwidget::receive);
    connect(socket->base(), &QAbstractSocket::disconnected, [=]() {
        QMessageBox::critical(this, tr("Connection lost"), tr("Connection to server has closed"));
    });
    connect(socket, SIGNAL(NetworkSocket::connected), this, SLOT(setConnected));
    connect(socket, SIGNAL(NetworkSocket::error0occured), this, SLOT(setUnconnected));
    ui->label_2->hide();
    socket->hello(IP,PORT);


    if(isConnected)
        ui->label->setText("Welcome, "+username);
    else
        ui->label->setText("Connection Failed, Dear "+username);
    qDebug() << (isConnected?"yes":"no");
}

bool mul_initwidget::isValidID(QString *ID) {
    std::string s = ID->toStdString();
    for (auto i : s) {
        if(!isalnum(i) && i != '_')
            return false;
    }
    return true;
}

void mul_initwidget::on_pushButtonJoin_clicked()
{
    QString roomID = ui->lineEdit->text();
    if(!isConnected) socket->hello(IP,PORT);
    if(!isValidID(&roomID) || !isConnected) {
        ui->label_2->show();
        return;
    }
    NetworkData networkData(OPCODE::JOIN_ROOM_OP, roomID, username);
    socket->send(networkData);

}
void mul_initwidget::on_pushButtonNew_clicked()
{
}

void mul_initwidget::setConnected()
{
    isConnected = true;
}
void mul_initwidget::setDisconnected()
{
    isConnected = false;
}

void mul_initwidget::receive(NetworkData data)
{
    switch(data.op) {
    case OPCODE::JOIN_ROOM_REPLY_OP:
        ui->label_2->hide();
        emit enterRoom(ui->lineEdit->text(), socket->base(), IP, PORT, data, username);
        break;
    case OPCODE::ERROR_OP: //unfinished
        break;
    default:
        qDebug() << "Error Receiving " << static_cast<int>(data.op);
        break;
    }
    hide();
}

mul_initwidget::~mul_initwidget()
{
    socket->bye();
    delete ui;
    delete socket;
}
