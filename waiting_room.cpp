#include "waiting_room.h"
#include "ui_waiting_room.h"
#include "networkdata.h"

WaitingRoom::WaitingRoom(QWidget *parent):
    QWidget(parent), ui(new Ui::WaitingRoom)
{
    setWindowTitle("准备房间");
    ui->setupUi(this);
    labelStack = new QLabel*[6];
    labelStack[0] = ui->playerLabel_1;
    labelStack[1] = ui->playerLabel_2;
    labelStack[2] = ui->playerLabel_3;
    labelStack[3] = ui->playerLabel_4;
    labelStack[4] = ui->playerLabel_5;
    labelStack[5] = ui->playerLabel_6;
}

void WaitingRoom::initWindow(QString roomID, QTcpSocket *Tcp, QString hostIP, int hostPort, NetworkData roomState, QString username) {
    this->roomID = roomID;
    socket = new NetworkSocket(Tcp, this);
    connect(socket, &NetworkSocket::receive, this, &WaitingRoom::receive);
    connect(socket->base(), &QAbstractSocket::disconnected, [=]() {
        QMessageBox::critical(this, tr("Connection lost"), tr("Connection to server has closed"));
    });
    socket->hello(hostIP, hostPort);
    _label_setPostfix(ui->label_roomID, roomID);
    playerNum = 0;
    QStringList playerInfo = roomState.data1.split(" ", Qt::SkipEmptyParts);
    foreach(QString name, playerInfo) {
        labelStack[playerNum]->setText(name+(roomState.data2[playerNum] == '1'?PLAYER_READY:PLAYER_PREPARING));
        playerNum++;
    }
    labelStack[playerNum++]->setText(username + PLAYER_PREPARING);
    this->username = username;
    show();
}

void WaitingRoom::receive(NetworkData data)
{
    qDebug() << static_cast<int>(data.op);
    auto findPlayer = [this](QString playerID) -> int {
        for(int i = 0; i<playerNum; ++i) {
            if(labelStack[playerNum]->text() == playerID) {
                return i;
            }
        }
        throw -1;
    };
    auto EmitStart = [this,&data]() -> void {
        auto ls = data.data1.split(" ");
        std::vector<std::pair<QString,QString>> Vec;
        std::map<QString,bool> flags;
        int s = 0;
        foreach(QString str,ls) {
            Vec.push_back(std::make_pair(str, data.data2[2*s]));
            s++;
            if (str == username) flags[str] = true;
            else flags[str] = false;
        }
        if(s != playerNum) throw 114514;
        emit start(s, &Vec, &flags);
        emit start_netini(socket->base());
        return;
    };
    try {
        switch (data.op){
            case OPCODE::LEAVE_ROOM_OP: //A player leaves the room
                //move all other players forward.
                for(int i = findPlayer(data.data1); i+1<playerNum; ++i)
                    labelStack[i]->setText(labelStack[i+1]->text());
                //remove the player
                labelStack[--playerNum]->setText(PLAYER_WAITING);
                break;
            case OPCODE::PLAYER_READY_OP:
                _label_setPostfix(labelStack[findPlayer(data.data1)], PLAYER_READY);
                break;
            case OPCODE::START_GAME_OP:
                EmitStart();
                close();
                break;
            default:
                qDebug() << "Unexpected signal received.";
                throw 404;
        }
    }catch (int t) {
        qDebug() << "Exception with code " << t;
    }
    return;
}

void WaitingRoom::send_ready() {
    socket->send(NetworkData(OPCODE::PLAYER_READY_OP, username, ""));
}

void WaitingRoom::_label_setPostfix(QLabel *label, const QString str)
{
    QString tmp = label->text();
    std::string ss = tmp.toStdString();
    ss.erase(ss.find('('));
    tmp.fromStdString(ss);
    tmp += str;
    label->setText(tmp);
}


WaitingRoom::~WaitingRoom()
{
    delete ui;
}

const QString WaitingRoom::PLAYER_WAITING = "Waiting";
const QString WaitingRoom::PLAYER_READY = "(Ready)";
const QString WaitingRoom::PLAYER_PREPARING = "PREPARING";

