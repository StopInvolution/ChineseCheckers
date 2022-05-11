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
        if(invalidName(data.data1)) {   //check if username is absloutly unacceptable.
            server->send(client, NetworkData(OPCODE::ERROR_OP, "INVALID_JOIN", "invalid username. Please use another one"));
            return 0;
        }
        Room *room;
        room = findRoom(data.data2);
        if(room == NULL) {  //A new room should be created if not found.
            room = new Room(data.data2);
            roomList.push_back(room);
        }

        for(auto i:room->chessboard->players) {
            server->send(i->getSocket(), NetworkData(OPCODE::JOIN_ROOM_OP, data.data1, ""));
        }
        server->send(client, NetworkData(OPCODE::JOIN_ROOM_REPLY_OP, room->playerNameListStr(), ""));
        room->addPlayer(new ServerPlayer(data.data1, client));
        this->ui->textBrowser->append("send: JOIN_ROOM_REPLY_OP");
        break;
    case OPCODE::LEAVE_ROOM_OP:
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

Room* ServerWidget::findRoom (QString &roomName)
{
    // future plan: using a better search algorithm. Maybe a hash will work.
    // anyway, there's no need optimizing it currently...
    for (auto i:roomList) {
        if(i->RoomID() == roomName)
        {
            return i;
        }
    }
    return NULL;
}

void ServerWidget::__FakeData()
{
    QString words = ui->textEdit->toPlainText();
    ui->textEdit->clear();
    auto list = words.split(" ", Qt::SkipEmptyParts);
    if(list[0] == "JOIN_ROOM_OP") {
        auto room = roomList[0];
        for(auto i:room->chessboard->players) {
            server->send(i->getSocket(), NetworkData(OPCODE::JOIN_ROOM_OP, list[1], ""));
        }
        //server->send(client, NetworkData(OPCODE::JOIN_ROOM_REPLY_OP, room->playerNameListStr(), ""));
        room->addPlayer(new ServerPlayer(list[1]));
    }
    return;
}
