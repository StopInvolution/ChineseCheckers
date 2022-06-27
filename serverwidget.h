#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <ctime>
#include "widget.h"
#include "room.h"
#include "networkserver.h"
#include "networkdata.h"

namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT
    QVector<Room*> roomList;
    NetworkServer *server;

public:
    explicit ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();

private:

    void write(std::string);
    Room *findRoom(QString &);
    static bool invalidName(QString &name);
    Ui::ServerWidget *ui;
    int moveQuery(ChessBoard* chessBoard, QString data1, QString data2);

private slots:
    /**
     * @arg client : source of the message.
     *      data   : the message itself.
     */
    int receiveData(QTcpSocket *client, NetworkData data);
    void __receiveCommand();
public slots:
    /**
     * @brief when players get overtimed, send essential message and continue the game.
     */
    void overtime(QString c);
    /**
     * @brief bring an end to the game and delete the room.
     */
    void endGame(QString data);
    /**
     * @brief send Victory message to the player
     * @param name:player's name
     */
    void sendVictory(QString name);
    void startTurn(QString name);
};

#endif // SERVERWIDGET_H
