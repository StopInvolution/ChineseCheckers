#ifndef ROOM_H
#define ROOM_H

#include <QString>
#include "chessboard.h"
#include "player.h"
#include "serverplayer.h"
#include "widget.h"

class Room
{
    QString roomID;
    int gameRunning;
public:
    QVector<ServerPlayer*> players;
//    ChessBoard* chessboard;
    /**
    * @return the ID of the room.
    */
    QString RoomID() const;
    /**
    * @return players' name in the form of "player1 player2 ... playerN"
    *        note that there's no space at the end of string.
    */
    QString playerNameListStr(char sep = ' ') const;
    /**
    * @return players' state(ready or not) in the form of "state1state2...stateN"
    */
    QString playerStateListStr() const;
    /**
    * @brief add a player into playerlist
    */
    void addPlayer(ServerPlayer* player);
    /**
     * @brief remove player from the room
     * @param playerIndex: player's index in vector
     */
    void removePlayer(int playerIndex);
    /**
     * @brief remove player from the room
     * @param player: player's address
     */
    void removePlayer(ServerPlayer* player);
    /**
     * @return if the game is running, returns 1; else returns 0.
     */
    bool isGameRunning();
    /**
     * @brief change state of this game: switch between running and waiting.
     */
    void changeGameState();
    Room();
    Room(QString RoomID);

    Widget* w;
    ~Room();
};

#endif // ROOM_H
