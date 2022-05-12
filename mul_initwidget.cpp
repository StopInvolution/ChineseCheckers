#include "mul_initwidget.h"
#include "ui_mul_initwidget.h"

const QString mul_initwidget::IP = "127.0.0.1";

mul_initwidget::mul_initwidget(QWidget *parent) :
    QWidget(parent),
    isConnected(false),
    ui(new Ui::mul_initwidget),
    username("user114514")
{
    ui->setupUi(this);
    this->socket = new NetworkSocket(new QTcpSocket(), this);
    qDebug()<<"mul_init create socket at" << (void*)&socket;
    connect(socket, &NetworkSocket::receive, this, &mul_initwidget::receive);
    //connect(socket->base(), &QAbstractSocket::disconnected, [=]() {
    //    QMessageBox::critical(this, tr("Connection lost"), tr("Connection to server has closed"));
    //});
    connect(socket->base(), SIGNAL(connected()), this, SLOT(setConnected()));
    connect(socket->base(), SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(setDisconnected(QAbstractSocket::SocketError)));
    //connect(socket->base(), SIGNAL(hostFound()), this, SLOT(debug()));
    ui->label_2->hide();
    socket->hello(IP,PORT);

    if(isConnected)
        ui->label->setText("Welcome, "+username);
    else
        ui->label->setText("Unconnected.");
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
    ui->label->setText("Welcome, "+username);
}
QAbstractSocket::SocketError mul_initwidget::setDisconnected(QAbstractSocket::SocketError Error)
{
    isConnected = false;
    ui->label->setText("Unconnected.");
    qDebug() << "Error";
    return Error;
}

void mul_initwidget::receive(NetworkData data)
{
    switch(data.op) {
    case OPCODE::JOIN_ROOM_REPLY_OP:
        ui->label_2->hide();
        emit enterRoom(ui->lineEdit->text(), socket, data, username);
        disconnect(socket, &NetworkSocket::receive, this, &mul_initwidget::receive);
        this->close();
        break;
    case OPCODE::ERROR_OP: //unfinished
    default:
        qDebug() << "Mul_initWidget Receiving " << static_cast<int>(data.op);
        break;
    }
}

mul_initwidget::~mul_initwidget()
{
    qDebug() << "delete";
    disconnect(socket->base(), &QAbstractSocket::disconnected, 0, 0);
    socket->bye();
    delete ui;
    delete socket;
}

void mul_initwidget::debug() {
    qDebug() << "QaQ";
}
