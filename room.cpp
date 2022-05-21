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
}


QString Room::RoomID() const
{
    return this->roomID;
}

QString Room::playerNameListStr() const
{
    QString tmp("");
    for(auto i:players) {
        if(!tmp.isEmpty()) tmp.append(' ');
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
        for(size_t i = 0; i < players.size(); ++i){
            QString s=getID(board::playerSpawn[players.size()][i]);
            Vec.push_back(std::make_pair(players[i]->name, s));
        }
        w->setChessBoard(players.size(), &Vec, &m, 0);
        this->w->show();
    }
}

void Room::removePlayer(int index) {
    assert(index >= 0);
    for(size_t i = index+1; i < players.size(); ++i) {
        players[i-1] = players[i];
    }
    players.pop_back();
}

void Room::removePlayer(ServerPlayer *player) {
    for (auto i = players.begin(); i != players.end(); ++i) {
        if (*i == player) {
            players.erase(i);
            return;
        }
    }
}

Room::~Room()
{
    delete w;
    for(auto i:players) {
        ServerPlayer* p = (ServerPlayer*)(void*)i;
        delete p;
    }
}
