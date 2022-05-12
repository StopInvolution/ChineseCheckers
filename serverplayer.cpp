#include "serverplayer.h"

ServerPlayer::ServerPlayer(int _color, int _spawn, int _target, unsigned int _flag, QString _name, int _chessNum, bool isready):
    Player(_color, _spawn, _target, _flag, _name, _chessNum),
    isready(isready)
{
}

ServerPlayer::ServerPlayer(QString name)
    :ServerPlayer()
{
    this->name = name;
}

ServerPlayer::ServerPlayer(QString name, QTcpSocket* socket):
    ServerPlayer(name)
{
    assert(socket != nullptr);
    client = socket;
}

bool ServerPlayer::isReady() const
{
    return isready;
}

QTcpSocket* ServerPlayer::getSocket() const
{
    return client;
}

void ServerPlayer::Ready() {isready = true;}

ServerPlayer::~ServerPlayer()
{

}
