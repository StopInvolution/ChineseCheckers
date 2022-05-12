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

Room::~Room()
{
    delete chessboard;
    for(auto i:players) {
        ServerPlayer* p = (ServerPlayer*)(void*)i;
        delete p;
    }
}
