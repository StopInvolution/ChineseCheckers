#ifndef WAITING_ROOM_H
#define WAITING_ROOM_H

#include <qtableview.h>
#include <QWidget>

namespace Ui{
class WaitingRoom;
}

class WaitingRoom : public QWidget {
    Q_OBJECT

    Ui::WaitingRoom* ui;
    int spinBoxVal;
public:
    explicit WaitingRoom(QWidget *parent = nullptr);
    ~WaitingRoom();

public slots:
    void initWindow(int player);

private slots:
    void spinBoxRecheck(int val);
};

#endif // WAITING_ROOM_H
