#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
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
    Room *findRoom(QString &);
    static bool invalidName(QString &name);
    Ui::ServerWidget *ui;

private slots:
    int receiveData(QTcpSocket *client, NetworkData data);
    void __receiveFakeData();
public slots:
    void overtime(QString c);
    void endGame(QString data);
    void sendVictory(QString name);
    void startTurn(QString name);
};

#endif // SERVERWIDGET_H
