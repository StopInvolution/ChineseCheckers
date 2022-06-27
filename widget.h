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

/**
 * @brief The Widget class 是棋盘的显示窗口
 */
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
    /**
     * @brief initChessBoard 给定棋盘信息，创建（重建）棋盘
     * @param kind 棋盘类别
     * @param newPlayerNum 玩家总数
     * @param playerInfo 玩家信息，{玩家：位置编号}键值对
     * @param localFlag 本地玩家标记
     * @param _socket 可空，客户端socket
     */
    void initChessBoard(int kind,int newPlayerNum=2,QVector<pss>* playerInfo=nullptr, std::map<QString,bool>* localFlag=nullptr, NetworkSocket* _socket=nullptr);

public:
    /**
     * @brief setChessBoard 给定棋盘信息，创建（重建）棋盘
     * @param kind 棋盘类别
     * @param newPlayerNum 玩家总数
     * @param playerInfo 玩家信息，{玩家：位置编号}键值对
     * @param localFlag 本地玩家标记
     * @param _socket 可空，客户端socket
     */
    void setChessBoard(int kind,int newPlayerNum=2,QVector<pss>* playerInfo=nullptr, std::map<QString,bool>* localFlag=nullptr, NetworkSocket* _socket=nullptr);
    void on_btnSetPlayerNum_clicked();
    void closeEvent(QCloseEvent *);

//private:
    Ui::Widget *ui;

public: signals:
    void backToTitle();
};
#endif // WIDGET_H
