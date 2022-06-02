#ifndef SERVERPLAYER_H
#define SERVERPLAYER_H
#include <QTcpSocket>
#include "player.h"

class ServerPlayer : public Player
{
    bool isready;
    QTcpSocket* client;
public:
    QString startArea;
    ServerPlayer(QString Name);
    ServerPlayer(QString Name, QTcpSocket* socket);
    ServerPlayer(int _color = 1, int _spawn = 0, int _target = 3,
                 unsigned int _flag=2, QString _name = "张三", int _chessNum = 10, bool isready=0);
    ~ServerPlayer();

    bool isReady() const override;
    void Ready();
    QTcpSocket* getSocket() const override;
};

#endif // SERVERPLAYER_H
