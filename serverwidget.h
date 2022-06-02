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

private slots:
    int receiveData(QTcpSocket *client, NetworkData data);
    void __receiveCommand();
public slots:
    void overtime(QString c);
    void endGame(QString data);
    void sendVictory(QString name);
    void startTurn(QString name);
};

#endif // SERVERWIDGET_H
