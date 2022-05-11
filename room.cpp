#include "room.h"

Room::Room()
{
}
Room::Room(QString RoomID)
    :roomID(RoomID), chessboard()
{
}


QString Room::RoomID() const
{
    return this->roomID;
}

QString Room::playerNameListStr() const
{
    QString tmp("");
    for(auto i:chessboard->players) {
        if(!tmp.isEmpty()) tmp.append(' ');
        tmp.append(i->name);
    }
    return tmp;
}

QString Room::playerStateListStr() const
{
    QString tmp("");
    for(auto i:chessboard->players) {
        if(i->isReady()) tmp.append("1");
        else tmp.append("0");
    }
    return tmp;
}

void Room::addPlayer(ServerPlayer *player) {
    chessboard->players.push_back(player);
}

Room::~Room()
{
    delete chessboard;
}
