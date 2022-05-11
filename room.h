#ifndef ROOM_H
#define ROOM_H

#include <QString>
#include "chessboard.h"
#include "player.h"
#include "serverplayer.h"

class Room
{
    QString roomID;


public:
    ChessBoard* chessboard;
    /**
    * @brief returns the ID of the room.
    */
    QString RoomID() const;
    /**
    * @brief return players' name in the form of "player1 player2 ... playerN"
    *        note that there's no space at the end of string.
    */
    QString playerNameListStr() const;
    /**
    * @brief return players' state(ready or not) in the form of "state1state2...stateN"
    */
    QString playerStateListStr() const;

    void addPlayer(ServerPlayer* player);

    Room();
    Room(QString RoomID);

    ~Room();
};

#endif // ROOM_H
