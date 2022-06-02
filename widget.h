#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QTimer>
#include <QPushButton>
#include <QLineEdit>
#include "marble.h"
#include "settings.h"
#include "networksocket.h"
#include "networkdata.h"
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class ChessBoard;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    NetworkSocket* socket;

    int playerNum;
    ChessBoard *chessBoard;

    QPushButton *btnSetPlayerNum;
    QLineEdit *editPlayerNum;
    QLabel *labelPlayerNum;

public slots:
    void initChessBoard(int newPlayerNum=2,QVector<pss>* playerInfo=nullptr, std::map<QString,bool>* localFlag=nullptr, NetworkSocket* _socket=nullptr);

public:
    void setChessBoard(int newPlayerNum=2,QVector<pss>* playerInfo=nullptr, std::map<QString,bool>* localFlag=nullptr, NetworkSocket* _socket=nullptr);
    void on_btnSetPlayerNum_clicked();
    void closeEvent(QCloseEvent *);

//private:
    Ui::Widget *ui;

public: signals:
    void backToTitle();
};
#endif // WIDGET_H
