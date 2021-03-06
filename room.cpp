#include "room.h"
#include "util.h"
#include "settings.h"
#include "widget.h"

Room::Room()
{
}
Room::Room(QString RoomID)
    :roomID(RoomID), gameRunning(0)
{
    w=new Widget;
    w->hide();
    w->setWindowTitle("服务端棋盘");
}


QString Room::RoomID() const
{
    return this->roomID;
}

QString Room::playerNameListStr(char sep) const
{
    QString tmp("");
    for(auto i:players) {
        if(!tmp.isEmpty()) tmp.append(sep);
        tmp.append(i->name);
    }
    return tmp;
}

QString Room::playerStateListStr() const
{
    QString tmp("");
    for(auto i:players) {
        if(i->isReady()) tmp.append("1");
        else tmp.append("0");
    }
    return tmp;
}

void Room::addPlayer(ServerPlayer *player) {
    players.push_back(player);
}

bool Room::isGameRunning() {
    return gameRunning;
}

void Room::changeGameState() {
    gameRunning ^= 1;
    if(gameRunning == 1) {
        QVector<std::pair<QString,QString>> Vec;
        std::map<QString,bool> m;
        qDebug()<<m.size();
        for(qsizetype i = 0; i < players.size(); ++i){
            QString s=getID(board::playerSpawn[players.size()][i]);
            Vec.push_back(std::make_pair(players[i]->name, s));
        }
        w->setChessBoard(CB::SERVER, players.size(), &Vec, &m, 0);
        this->w->show();
    }else{
        assert(w != nullptr);
        w->close();
    }
}

void Room::removePlayer(int index) {
    assert(index >= 0);
    for(qsizetype i = index+1; i < players.size(); ++i) {
        players[i-1] = players[i];
    }
    players.pop_back();
}

void Room::removePlayer(ServerPlayer *player) {
    for (qsizetype i = 0; i < players.size(); ++i) {
        if (players[i] == player) {
            return;
        }
    }
}

Room::~Room()
{
    delete w;
    for(auto i:players) {
        delete i;
    }
}
