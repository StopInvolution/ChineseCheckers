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
    std::vector<Room*> roomList;
    NetworkServer *server;

public:
    explicit ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();

private:
    Room *findRoom(QString ID);
    static bool invalidName(QString &name);
    Ui::ServerWidget *ui;

private slots:
    int receiveData(QTcpSocket *client, NetworkData data);
};

#endif // SERVERWIDGET_H
