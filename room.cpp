#include "room.h"

Room::Room()
{
}
Room::Room(QString RoomID)
    :roomID(RoomID), chessboard(new ChessBoard())
{
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
}

void Room::removePlayer(int index) {
    assert(index >= 0);
    for(int i = index+1; i < players.size(); ++i) {
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
    delete chessboard;
    for(auto i:players) {
        ServerPlayer* p = (ServerPlayer*)(void*)i;
        delete p;
    }
}
