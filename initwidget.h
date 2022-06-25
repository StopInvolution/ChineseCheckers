#ifndef INITWIDGET_H
#define INITWIDGET_H

#include <QWidget>
#include "networksocket.h"
#include "settings.h"

namespace Ui {
class initWidget;
}

class initWidget : public QWidget
{
    Q_OBJECT

public:
    explicit initWidget(QWidget *parent = nullptr);
    ~initWidget();

    signals:
    void start(int kind,int playerNum,QVector<pss>* playerInfo,std::map<QString,bool>* localFlag, NetworkSocket* _socket);
    void startOnline(int playerNum);
    void backToTitle();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::initWidget *ui;
};

#endif // INITWIDGET_H
