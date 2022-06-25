#ifndef MUL_INITWIDGET_H
#define MUL_INITWIDGET_H

#include "networkdata.h"
#include "networksocket.h"
#include <QWidget>
#include "settings.h"
#include "util.h"
#include <QMessageBox>
#include "settingswidget.h"

namespace Ui {
class mul_initwidget;
}

class mul_initwidget : public QWidget
{
    Q_OBJECT

public:
    explicit mul_initwidget(QWidget *parent = nullptr);
    ~mul_initwidget();

    bool isConnected;

private:
    Ui::mul_initwidget *ui;
    QString username;
    NetworkSocket* socket;

private:
    /**
     * @brief return if the username contains num/alpha/underscore and no longer than 20
    */
    static bool isValidID(QString *);
private slots:
    void debug();
    //send a request to the server.
    void on_pushButtonNew_clicked();
    //not finished.
    void on_pushButtonJoin_clicked();
    void on_pushButton_rename_clicked();
    void on_pushButtonSettings_clicked();
    void setConnected();
    QAbstractSocket::SocketError setDisconnected(QAbstractSocket::SocketError error);
    /**
     * @brief called when new data received
     * @param data : from the internet
     */
    void receive(NetworkData data);

public:
signals:
    /**
     * @brief emit to enter waiting room
     * @param roomID
     * @param sck = socket
     * @param roomState
     * @param username
     */
    void enterRoom(QString roomID, NetworkSocket* sck, NetworkData roomState, QString username);
    void backToTitle();
};

#endif // MUL_INITWIDGET_H
