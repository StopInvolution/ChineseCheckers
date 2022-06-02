#include "mul_initwidget.h"
#include "ui_mul_initwidget.h"

using namespace Network;

mul_initwidget::mul_initwidget(QWidget *parent) :
    QWidget(parent),
    isConnected(false),
    ui(new Ui::mul_initwidget),
    username("user")
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
    QString IP = ip[0]+":"+ip[1]+":"+ip[2]+":"+ip[3];
    socket->hello(IP,port);

    if(isConnected)
        ui->label->setText("Welcome, "+username);
    else
        ui->label->setText("Unconnected.");

    connect(ui->pushButtonExit, &QAbstractButton::clicked, [this]() {
        emit backToTitle();
        close();
        return;
    });
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
    QString IP = ip[0]+":"+ip[1]+":"+ip[2]+":"+ip[3];
    if(!isConnected) socket->hello(IP,port);
    if(!isValidID(&roomID) || !isConnected) {
        ui->label_2->show();
        return;
    }
    NetworkData networkData(OPCODE::JOIN_ROOM_OP, roomID, username);
    socket->send(networkData);
}
void mul_initwidget::on_pushButtonNew_clicked()
{
    QString IP = ip[0]+":"+ip[1]+":"+ip[2]+":"+ip[3];
    if(!isConnected) socket->hello(IP,port);
    if(!isConnected) {
        ui->label_2->show();
        return;
    }
    QString roomID = QString::number((int)(rand()*1.0/RAND_MAX*1000000), 16);
    NetworkData networkData(OPCODE::JOIN_ROOM_OP, roomID, username);
    socket->send(networkData);
}
void mul_initwidget::on_pushButton_rename_clicked() {
    if(this->ui->nameEdit->text()!=""){
        username=this->ui->nameEdit->text();
    }
    if(isConnected) ui->label->setText("Welcome, "+username);
}
static SettingsWidget *widget = nullptr;
void mul_initwidget::on_pushButtonSettings_clicked() {
    if(widget != nullptr) delete widget;
    widget = new SettingsWidget();
    widget->init();
}
void mul_initwidget::setConnected()
{
    isConnected = true;

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
        if(data.data1 == convertToQStr(ERRCODE::INVALID_JOIN) || data.data1 == convertToQStr(ERRCODE::ROOM_IS_RUNNING)){
            on_pushButtonNew_clicked();
            break;
        }
        break;
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
