#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <networksocket.h>
#include "settings.h"
#include <vector>
#include <QTimer>
#include "networkdata.h"
#include "mvector.h"
#include "clickableqlabel.h"


class Player;
class Marble;
class Widget;
class ClientWidget;

class AgentThread:public QThread{
public:
    Agent_algorithm *algo;
    QVector<AlgoPlayer> data;
    pcc res;
    AgentThread();
    AgentThread(Agent_algorithm _algo,QVector<AlgoPlayer> _data);
    void set(Agent_algorithm _algo,QVector<AlgoPlayer> _data);
protected:
    void run() override;
};

/**
 * @brief 本地棋盘，承载了所有跳棋游戏逻辑，是基类棋盘
 * @note 不包含重新设置玩家数量和重开的逻辑
 */
class ChessBoard:public QObject
{
    Q_OBJECT
public:
    ChessBoard(Widget *_parentWindow = 0, int _player_num=6,QVector<pss>* playerInfo=nullptr,std::map<QString,bool>* localFlag=nullptr);
    virtual ~ChessBoard();
    Widget *parentWindow;

    ClientWidget *console;
    int rotateAngle;
    int playerNum;
    int stepNum;
    QTimer *timeoutTimer= new QTimer;
    int clockT;
    const double initResTime;
    const clock_t initAgentCD;
    clock_t agentCD;
    double resTime,tick;
    bool god;
    bool movePermission;
    bool onAgent,onSingleAgent;
    QString gameResult;

    AgentThread *agT;

    // 当前行棋方
    int activatedPlayerID;
    Player *activatedPlayer;

    // 选取后待移动的子
    Marble *selectedChess;

    /**
     * @brief occupiedPst 是已被填充的位置的标记，注意下标偏移一个边界长度
     */
    bool occupiedPst[2*board::indexBoundary+1][2*board::indexBoundary+1];

    // 玩家
    QVector<Player*> players;
    QVector<Player*> winnerRank;
    QVector<Player*> outer;

    // hint 抽象成一个玩家
    Player * hintPlayer;

    /**
     * @brief labelInfo 是信息显示面板
     */
    QLabel *labelInfo;

    /**
     * @brief 计算 selectedChess 可落子的位置，生成到 hintPlayer->hintChess
     */
    void getHint();

    /**
     * @brief 显示提示棋子
     */
    void showHint();

    /**
     * @brief 隐藏提示棋子
     */
    void unshowHint();

    /**
     * @brief 获取当前行动玩家的位置编号
     */
    QString getActID();

    /**
     * @brief 棋子被点击时的回调函数，用于选择待移动的普通棋子
     */
    void chooseChess (Marble *chess);

    /**
     * @brief 落点(hint)被点击时的回调函数，用于移动选择的普通棋子到该棋子的位置
     */
    virtual void moveChess(Marble *dest,QVector<ChessPosition> *path=nullptr);

    /**
     * @brief 对当前行动玩家执行超时判负，包括移除棋子和标记
     */
    virtual void timeout();

    /**
     * @brief 展示排名，形式是提示框
     * @param data 用空格隔开的位置编号
     */
    void showRank(QString data);

    /**
     * @brief 落子后，判断刚才这一落子是否使得刚才这个人完赛，并切换到下一个行动玩家
     */
    virtual void nextTurn();

    /**
     * @brief 切换到下一个行动玩家
     */
    void setNextActivatedPlayer();

    /**
     * @brief 切换到行动玩家为指定玩家
     * @param player 要切换为的指定玩家
     */
    void setActivatedPlayer(Player *player=nullptr);

    /**
     * @brief 判断两点间是否存在路径可以移动
     * @param p1 移动起点
     * @param p2 移动终点
     * @return 返回是否合法
     */
    bool checkMove(ChessPosition p1,ChessPosition p2);

    /**
     * @brief 尝试移动，失败的话会撤销操作
     * @param p1 移动起点
     * @param p2 移动终点
     * @return 返回是否合法
     */
    bool moveA2B(ChessPosition p1,ChessPosition p2);

    /**
     * @brief 根据路径移动
     * @param p 路径序列
     * @param ck 是否做合法性检查
     * @return 返回是否合法
     */
    bool moveA2BWithPath(QVector<ChessPosition>* p,bool ck=true);

    /**
     * @brief 为当前行动玩家随机走一步
     */
    void randomMove();

    QPushButton *btnRandomMove,*btnAutoMv,*btnStopAutoMv,*btnAIMv,*btnConsole;

    void on_btnRandomMove_clicked();
    void on_btnAutoMv_clicked();
    void on_btnStopAutoMv_clicked();
    void on_btnSetPlayerNum_clicked();
    void onConsole();

    /**
     * @brief 为当前行动玩家走一步AI
     */
    void agent_move();

    QVector<pss> initPlayerInfo;
    std::map<QString,bool> initLocalFlag;

    /**
     * @brief 更新信息栏 this->labelInfo
     */
    virtual void updateLabelInfo();

    /**
     * @brief 调用所有可显示对象的show()，除了提示棋子
     */
    void show();

    /**
     * @brief 根据位置获取棋子
     * @param p 棋子坐标
     * @parm playerID 指定从第几个玩家拥有的棋子中寻找
     */
    Marble* getChess(ChessPosition p,int playerID=-1);

    /**
     * @brief 根据位置获取棋子
     * @param x 棋子横坐标
     * @param y 棋子纵坐标
     * @parm playerID 指定从第几个玩家拥有的棋子中寻找
     */
    Marble* getChess(int x,int y,int playerID=-1);

    /**
     * @brief 根据玩家名获取玩家指针
     * @param name 玩家名
     * @return 玩家指针
     */
    Player* getPlayerByName(QString name);

    /**
     * @brief 根据玩家位置编号获取玩家指针
     * @param name 玩家位置编号
     * @return 玩家指针
     */
    Player *getPlayerByID(QString ID);

    ClickableQLabel *labelPlayer[7];
    QVector<AlgoPlayer> AIDataProducer();

    /**
     * @brief 判断当前行动玩家的编号和给定的是否对应
     * @param ID 用于比较的玩家位置编号
     * @return 是否对应
     */
    bool checkAct(QString ID="");

   signals:
    /**
     * @brief startTurn 是开始下一局的信号
     */
    void startTurn(QString);

    /**
     * @brief onMyTurn 是我能下棋的信号（包含了这局是我行棋的含义）
     */
    void onMyTurn();

    /**
     * @brief overtime 是超时信号
     * @param ID 超时玩家的位置编号
     */
    void overtime(QString ID);

    /**
     * @brief victory 是获胜信号
     * @param ID 超时玩家的名字
     */
    void victory(QString name);

    /**
     * @brief endgame 是游戏结束信号
     * @param data 是排名信息，用空格隔开的位置编号
     */
    void endgame(QString data);
};

/**
 * @brief The ServerChessBoard class 是服务器棋盘，不自动执行 NextTurn，但 NextTurn 会发送信号，接受且只接受网络传输协议类型的行棋指令
 */
class ServerChessBoard: public ChessBoard{
    using ChessBoard::ChessBoard;
    virtual void moveChess(Marble* dest,QVector<ChessPosition> *path) override;
    virtual void timeout() override;
    virtual void nextTurn() override;
};

/**
 * @brief The SocketChessBoard class 是带 Socket 的服务端棋盘，接受且只接受网络传输协议类型的行棋指令
 */
class SocketChessBoard: public ChessBoard{
public:
    SocketChessBoard(Widget *_parentWindow = 0, int _player_num=6,QVector<pss>* playerInfo=nullptr,std::map<QString,bool>* localFlag=nullptr,NetworkSocket* _socket=nullptr);
    virtual ~SocketChessBoard();
    NetworkSocket *socket;

    /**
     * @brief nertworkProcess 处理网络指令
     * @param data 网络数据
     */
    void nertworkProcess(NetworkData data);

    // 以下是重写
    virtual void moveChess(Marble* dest,QVector<ChessPosition> *path) override;
    virtual void nextTurn() override;
    virtual void updateLabelInfo() override;
};

/**
 * @brief 判断在给定棋盘上，三个点之间有没有别的棋子
 * @param chessBoard 所属棋盘
 * @param u 第一个棋子
 * @param mid 第二个棋子
 * @param v 第三个棋子
 * @return 是否有
 */
bool isAnyChessBetween(ChessBoard* chessBoard, ChessPosition u, ChessPosition mid, ChessPosition v);
#endif // CHESSBOARD_H
