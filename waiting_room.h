#ifndef WAITING_ROOM_H
#define WAITING_ROOM_H

#include <qtableview.h>
#include <QWidget>
#include "networkdata.h"
#include <QMessageBox>
#include "networksocket.h"

namespace Ui{
class WaitingRoom;
}

class WaitingRoom : public QWidget {
    Q_OBJECT

    static const QString PLAYER_WAITING;
    static const QString PLAYER_READY;
    static const QString PLAYER_PREPARING;
    Ui::WaitingRoom *ui;
    NetworkSocket* socket;
    QString roomID;
    int playerNum;
    QLabel **labelStack;
    QString username;
private:
    /**
    * @brief Remove the existing postfix and change it
    * @param label the pointer of the label you want to operate
    * @param str the postfix you need
    */
    void _label_setPostfix(QLabel* label, const QString str);
signals:
    void __backToTitle();

public:
    explicit WaitingRoom(QWidget *parent = nullptr);
    ~WaitingRoom();
signals:
    void start(int number, std::vector<std::pair<QString,QString>>* playerInfo, std::map<QString,bool>* localFlag);
    void start_netini(QTcpSocket *sck);

public slots:
    /**
     * @brief initalize window, and show it when connection succeed.
    */
    void initWindow(QString roomID, NetworkSocket* sck, NetworkData data, QString username);
    void backToTitle();

private slots:
    void receive(NetworkData data);

    void send_ready();
};

#endif // WAITING_ROOM_H
