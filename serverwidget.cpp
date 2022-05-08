#include "serverwidget.h"
#include "ui_serverwidget.h"

ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    ui->setupUi(this);
    this->server = new NetworkServer(this);

    connect(this->server, &NetworkServer::receive, this, &ServerWidget::receiveData);
}

int ServerWidget::receiveData(QTcpSocket *client, NetworkData data) {
    switch(data.op) {
    case OPCODE::JOIN_ROOM_OP:
        if(invalidName(data.data1)) {
            server->send(client, NetworkData(OPCODE::ERROR_OP, "INVALID_JOIN", "invalid username. Please use another one"));
            return 0;
        }
        //Room *room = findRoom(data.data2);
        break;
    default:
        break;

    }

    return 1;
}

ServerWidget::~ServerWidget()
{
    delete ui;
}

bool ServerWidget::invalidName(QString &name)
{
    if(name.size() > 20) return true;
    std::string s = name.toStdString();
    for(auto i:s)
        if (!isalnum(i) && i != '_')
             return true;
    return false;
}

Room* findRoom (QString &roomName)
{
    qDebug() << roomName;
    return NULL;
}
