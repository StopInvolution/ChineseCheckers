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

/**
 * @brief The AgentThread class 是托管线程类，一般由棋盘创建，并绑定结束事件从而调用计算结果。
 */
class AgentThread:public QThread{
public:
    /**
     * @brief algo 是托管算法
     */
    Agent_algorithm *algo;
    /**
     * @brief data 是托管数据，可以由棋盘的 AIDataProducer() 方法生成
     */
    QVector<AlgoPlayer> data;
    /**
     * @brief res 是运算结果，即行棋起点和终点
     */
    pcc res;
    AgentThread();
    AgentThread(Agent_algorithm _algo,QVector<AlgoPlayer> _data);
    /**
     * @brief set 加载托管算法和数据
     * @param _algo 托管算法
     * @param _data 托管数据
     */
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
    /**
     * @brief parentWindow 是父显示窗口指针
     */
    Widget *parentWindow;

    /**
     * @brief console 是控制台指针
     */
    ClientWidget *console;

    /**
     * @brief agT 是托管线程
     */
    AgentThread *agT;

    /**
     * @brief labelInfo 是信息显示面板
     */
    QLabel *labelInfo;

    /**
     * @brief btnRandomMove 是随机移动按钮
     */
    QPushButton *btnRandomMove;

    /**
     * @brief btnAutoMv 是开启托管按钮
     */
    QPushButton *btnAutoMv;

    /**
     * @brief btnStopAutoMv 是关闭托管按钮
     */
    QPushButton *btnStopAutoMv;

    /**
     * @brief btnAIMv 是使用一步托管按钮
     */
    QPushButton *btnAIMv;

    /**
     * @brief btnConsole 是打开控制台按钮
     */
    QPushButton *btnConsole;

    /**
     * @brief rotateAngle 是旋转角度
     */
    int rotateAngle;

    /**
     * @brief playerNum 是玩家人数
     */
    int playerNum;

    /**
     * @brief stepNum 是当前总步数
     */
    int stepNum;

    /**
     * @brief timeoutTimer 是超时事件计时器，用于更新倒计时并在超时时发出信号
     */
    QTimer *timeoutTimer= new QTimer;

    /**
     * @brief clockT 是超时计时器更新倒计时的周期
     */
    int clockT;

    /**
     * @brief initResTime 是倒计时总时长
     */
    const double initResTime;

    /**
     * @brief resTime 是本次倒计时所剩时间
     */
    double resTime;

    /**
     * @brief initAgentCD 是托管的 CD 时间，以ms记
     * @note 用于优化显示效果，避免本地游戏时瞬间结束游戏
     */
    const clock_t initAgentCD;

    /**
     * @brief agentCD 是上次使用托管的时间，以ms记
     */
    clock_t agentCD;

    /**
     * @brief god 是是否开启上帝模式
     */
    bool god;

    /**
     * @brief movePermission 是目前是否拥有移动棋子的权限
     */
    bool movePermission;

    /**
     * @brief onAgent 是是否处于托管状态
     */
    bool onAgent;

    /**
     * @brief onSingleAgent 是是否处于单步托管状态
     * @note 若是单步托管，会在行棋后自动解除 onAgent 和 onSingleAgent
     */
    bool onSingleAgent;

    /**
     * @brief gameResult 保存游戏结果排行榜，同时兼具判断游戏是否结束的功能，空则还在进行游戏
     */
    QString gameResult;

    /**
     * @brief activatedPlayerID 是当前行棋玩家序号
     */
    int activatedPlayerID;

    /**
     * @brief activatedPlayer 是当前行棋玩家指针
     */
    Player *activatedPlayer;

    /**
     * @brief selectedChess 是选取后待移动的棋子
     */
    Marble *selectedChess;

    /**
     * @brief occupiedPst 是已被填充的位置的标记，注意下标偏移一个边界长度
     */
    bool occupiedPst[2*board::indexBoundary+1][2*board::indexBoundary+1];

    /**
     * @brief players 是所有游戏玩家
     */
    QVector<Player*> players;

    /**
     * @brief hintPlayer 是管理提示棋子的玩家
     */
    Player * hintPlayer;

    /**
     * @brief winnerRank 是胜利者列表
     */
    QVector<Player*> winnerRank;

    /**
     * @brief outer 是出局者列表
     */
    QVector<Player*> outer;

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

    /**
     * @brief moveChess 通过且只通过路径来移动棋子，同时不自动调用 nextTurn
     * @param dest 应该填空，但为了接口一致性我们保留
     * @param path 是路径
     */
    virtual void moveChess(Marble* dest,QVector<ChessPosition> *path) override;

    /**
     * @brief timeout 同moveChess不自动调用 nextTurn
     */
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
 * @return 是否有别的棋子
 */
bool isAnyChessBetween(ChessBoard* chessBoard, ChessPosition u, ChessPosition mid, ChessPosition v);
#endif // CHESSBOARD_H
