#include "room.h"

Room::Room()
{
}

Room::Room(QString RoomID, std::vector<Player*> playerList)//:
    //roomID(RoomID), chessboard(new ChessBoard(NULL, playerList.size()))
{

}

Room::~Room()
{
    delete chessboard;
}
