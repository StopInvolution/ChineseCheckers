#include "serverwidget.h"
#include "ui_serverwidget.h"

ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    ui->setupUi(this);
    this->server = new NetworkServer(this);

    connect(this->server, &NetworkServer::receive, this, &ServerWidget::receiveData);
    //
}

int ServerWidget::receiveData(QTcpSocket *client, NetworkData data) {
    Room *room;
    bool flag;
    int result;
    QString ErrorCode;
    switch(data.op) {
    case OPCODE::MOVE_OP:
        room = roomList[0];
        if(room->isGameRunning() == false)
        {
            server->send(client, NetworkData(OPCODE::ERROR_OP, "ERRCODE::ROOM_NOT_RUNNING", ""));
            break;
        }
        result = room->chessboard->serverMoveProcess(data.data1, data.data2);
        switch(result) {
        case 1:
            for(auto i:room->players) {
                if(i->getSocket() == client) continue;
                server->send(i->getSocket(), data);
            }
            break;
        case 0:
        case -1:
            ErrorCode = (result == 0)?"INVALID_MOVE":"OUTTURN_MOVE";
            server->send(client, NetworkData(OPCODE::ERROR_OP, ErrorCode, ""));
            break;
        default:
            qDebug() << "ERROR at server widget";
        }
        break;
    case OPCODE::JOIN_ROOM_OP:
        this->ui->textBrowser->append("receive: JOIN_ROOM_OP");
        if(invalidName(data.data2)) {   //check if username is absloutly unacceptable.
            server->send(client, NetworkData(OPCODE::ERROR_OP, "INVALID_JOIN", "invalid username. Please use another one"));
            return 0;
        }
        room = findRoom(data.data1);
        if(room == NULL) {  //A new room should be created if not found.
            room = new Room(data.data1);
            roomList.push_back(room);
        }
        flag = false;
        for(auto i:room->players) { //Look for a duplicate username
            if(i->name == data.data2) {
                flag = true;
                server->send(client, NetworkData(OPCODE::ERROR_OP, "INVALID_JOIN", "Duplicate username."));
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
        this->ui->textBrowser->append("receive: LEAVE_ROOM_OP");
        room = nullptr;
        for(auto r:roomList) {
            if (r->RoomID() == data.data1) {
                room = r;
                break;
            }
        }
        if(room == nullptr) server->send(client, NetworkData(OPCODE::ERROR_OP, "INVALID_REQ", "room name not found"));
        flag = false;
        for(auto i:room->players) {
            if(i->name == data.data2) {
                flag = true;
                room->removePlayer(i);
                for(auto j:room->players) {
                    server->send(j->getSocket(), NetworkData(OPCODE::LEAVE_ROOM_OP, data.data2, ""));
                }
                break;
            }
        }
        if(!flag) server->send(client, NetworkData(OPCODE::ERROR_OP, "NOT_IN_ROOM", ""));
        break;
    case OPCODE::PLAYER_READY_OP:
        this->ui->textBrowser->append("receive: PLAYER_READY_OP");
        room = roomList[0];
        for(auto i:room->players) {
            i->Ready();
            server->send(i->getSocket(), NetworkData(OPCODE::PLAYER_READY_OP, data.data1, ""));
        }
        int N;
        N = room->players.size();
        if(N >= 2 && N != 5) {
            int count = 0;
            for(auto i:room->players)
                if (i->isReady()) count++;

            if(count == N) {
                this->ui->textBrowser->append("send: START_GAME_OP");
                for(auto i:room->players) {
                    server->send(i->getSocket(), NetworkData(OPCODE::START_GAME_OP,
                        room->playerNameListStr(), QString(std::string("A B C D E F").substr(0, 2*room->players.size()-1).c_str())));
                }
                room->changeGameState();
                connect(room->chessboard, &ChessBoard::overtime, this, &ServerWidget::overtime);
                connect(room->chessboard, &ChessBoard::endgame, this, &ServerWidget::endGame);
                connect(room->chessboard, &ChessBoard::victory, this, &ServerWidget::sendVictory);
            }
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
    if(list[0] == "join") {
        auto room = roomList[0];
        for(auto i:room->players) {
            server->send(i->getSocket(), NetworkData(OPCODE::JOIN_ROOM_OP, list[1], ""));
        }
        //server->send(client, NetworkData(OPCODE::JOIN_ROOM_REPLY_OP, room->playerNameListStr(), ""));
        room->addPlayer(new ServerPlayer(list[1]));
    }else if (list[0] == "size") {
        //qDebug() << server->clients.size();
    }else if (list[0] == "start") {
        auto room = roomList[0];
        QString p(std::string("A B C D E F").substr(0, 2*room->players.size()-1).c_str());
        for(auto i:room->players) {
            server->send(i->getSocket(), NetworkData(OPCODE::START_GAME_OP, room->playerNameListStr(), p));
            server->send(i->getSocket(), NetworkData(OPCODE::START_TURN_OP, "", ""));
        }
    }
    return;
}

void ServerWidget::overtime(QString data) {
    auto room = roomList[0];
    for(auto i:room->players) {
        server->send(i->getSocket(), NetworkData(OPCODE::MOVE_OP, data, "-1"));
    }
}

void ServerWidget::endGame(QString data)
{
    auto room = roomList[0];
    for(auto i:room->players) {
        server->send(i->getSocket(), NetworkData(OPCODE::END_TURN_OP, data, ""));
    }
}

void ServerWidget::sendVictory(QString name)
{
    auto room = roomList[0];
    for(auto i:room->players) {
        if(i->name == name)
            server->send(i->getSocket(), NetworkData(OPCODE::END_GAME_OP, "", ""));
    }
}
