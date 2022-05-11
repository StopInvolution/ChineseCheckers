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
    Room *findRoom(QString &);
    static bool invalidName(QString &name);
    Ui::ServerWidget *ui;

private slots:
    int receiveData(QTcpSocket *client, NetworkData data);
    void __FakeData();
};

#endif // SERVERWIDGET_H
