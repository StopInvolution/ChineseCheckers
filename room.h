#ifndef ROOM_H
#define ROOM_H

#include <QString>
#include "chessboard.h"
#include "player.h"

class Room
{
    QString roomID;
    ChessBoard* chessboard;

public:
    Room();
    Room(QString RoomID, std::vector<Player*> playerList);

    ~Room();
};

#endif // ROOM_H
