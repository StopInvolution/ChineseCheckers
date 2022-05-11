#ifndef MUL_INITWIDGET_H
#define MUL_INITWIDGET_H

#include "networkdata.h"
#include "networksocket.h"
#include <QWidget>
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
    static const QString IP;
    static const int PORT = 8000;

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
    void setConnected();
    QProcess::ProcessError setDisconnected(QProcess::ProcessError error);


    void receive(NetworkData data);

public:
signals:
    void enterRoom(QString roomID, QTcpSocket *sckt, NetworkData roomState, QString username);
};

#endif // MUL_INITWIDGET_H
