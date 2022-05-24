#ifndef MUL_INITWIDGET_H
#define MUL_INITWIDGET_H

#include "networkdata.h"
#include "networksocket.h"
#include <QWidget>
#include "settings.h"
#include <QMessageBox>

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
    static QString convertToQStr(ERRCODE err);
    Ui::mul_initwidget *ui;

    /* currently it's fixed. */
    QString username;
    NetworkSocket* socket;

private:
    /**/
    static bool isValidID(QString *);
private slots:
    void debug();
    //send a request to the server.
    void on_pushButtonNew_clicked();
    //not finished.
    void on_pushButtonJoin_clicked();
    void on_pushButton_rename_clicked();
    void setConnected();
    QAbstractSocket::SocketError setDisconnected(QAbstractSocket::SocketError error);

    void receive(NetworkData data);

public:
signals:
    void enterRoom(QString roomID, NetworkSocket* sck, NetworkData roomState, QString username);
};

#endif // MUL_INITWIDGET_H
