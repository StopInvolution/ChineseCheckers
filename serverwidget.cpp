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
    Room *room;
    switch(data.op) {
    case OPCODE::MOVE_OP:
        room = roomList[0];
        if(room->isGameRunning() == false)
        {
            //server->send(client, NetworkData(OPCODE::ERROR_OP, ERRCODE::ROOM_NOT_RUNNING, ));
            break;
        }
        //TODO: check if it's legal.
        for(auto i:room->players) {
            if(i->getSocket() == client) continue;
            server->send(i->getSocket(), NetworkData(OPCODE::MOVE_OP, data.data1, data.data2));
        }
        break;
    case OPCODE::JOIN_ROOM_OP:
        if(invalidName(data.data2)) {   //check if username is absloutly unacceptable.
            server->send(client, NetworkData(OPCODE::ERROR_OP, "INVALID_JOIN", "invalid username. Please use another one"));
            return 0;
        }
        room = findRoom(data.data1);
        if(room == NULL) {  //A new room should be created if not found.
            room = new Room(data.data1);
            roomList.push_back(room);
        }
        bool flag;
        flag = false;
        for(auto i:room->players) { //Look for a duplicate username
            if(i->name == data.data2) {
                flag = true;
                //server->send(client, NetworkData(OPCODE::ERROR_OP, ERRCODE::INVALID_JOIN, "Duplicate username."));
                break;
            }
        }
        if(flag) break;
        for(auto i:room->players) {
            server->send(i->getSocket(), NetworkData(OPCODE::JOIN_ROOM_OP, data.data2, ""));
        }
        server->send(client, NetworkData(OPCODE::JOIN_ROOM_REPLY_OP, room->playerNameListStr(), room->playerStateListStr()));
        room->addPlayer(new ServerPlayer(data.data2, client));
        this->ui->textBrowser->append("send: JOIN_ROOM_REPLY_OP");
        break;
    case OPCODE::LEAVE_ROOM_OP:
        room = nullptr;
        foreach(Room* r, roomList) {
            if (r->RoomID() == data.data1) {
                room = r;
                break;
            }
        }
        //if(room == nullptr) server->send(OPCODE::ERROR_OP, ERRCODE::INVALID_REQ, "room name not found");
        break;
    case OPCODE::PLAYER_READY_OP:
        room = roomList[0];
        for(auto i:room->players) {
            i->Ready();
            server->send(i->getSocket(), NetworkData(OPCODE::PLAYER_READY_OP, data.data1, ""));
        }
        break;
    default:
        server->send(client, NetworkData(OPCODE::ERROR_OP, "INVALID_REQ", ""));
        break;

    }
    return 0;
}

ServerWidget::~ServerWidget()
{
    for(auto i:roomList) delete i;
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
        for(auto i:room->players) {
            server->send(i->getSocket(), NetworkData(OPCODE::JOIN_ROOM_OP, list[1], ""));
        }
        //server->send(client, NetworkData(OPCODE::JOIN_ROOM_REPLY_OP, room->playerNameListStr(), ""));
        room->addPlayer(new ServerPlayer(list[1]));
    }else if (list[0] == "size") {
        //qDebug() << server->clients.size();
    }
    return;
}
