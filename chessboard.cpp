#include "chessboard.h"
#include <ctime>
#include <queue>
#include <random>
#include <stack>
#include "marble.h"
#include "player.h"
#include "settings.h"
#include "util.h"
#include "widget.h"

ChessBoard::ChessBoard(Widget* _parentWindow, int _player_num)
    : parentWindow(_parentWindow), playerNum(_player_num), stepNum(0), god(true), activatedPlayer(nullptr), selectedChess(nullptr) {
    srand(time(0));

    memset(this->occupiedPst, 0, sizeof(this->occupiedPst));

    for (int i = 0; i < playerNum; i++) {
        players.push_back(new Player(board::playerSpawn[playerNum][i], board::playerSpawn[playerNum][i], board::playerTarget[playerNum][i]));
        players.back()->addTo(this);
    }

    hintPlayer = new Player(color::hint, -1, -1, "hint", 0);
    hintPlayer->addTo(this);

    // 标签
    labelInfo = new QLabel(this->parentWindow);
    labelInfo->setGeometry(10, 10, 200, 80);
    labelInfo->setFont(QFont("华光中圆_CNKI", 16));

    setActivatedPlayer(this->players.front());
    this->activatedPlayerID = 0;

    updateLabelInfo();

    // 创建三个随机移动相关按钮
    btnRandomMove = new QPushButton(this->parentWindow);
    btnRandomMove->setGeometry(30, 210, 100, 30);
    btnRandomMove->setText("RandomMove");
    btnRandomMove->setCursor(Qt::PointingHandCursor);
    connect(this->btnRandomMove, &QPushButton::clicked, this, &ChessBoard::on_btnRandomMove_clicked);

    // 给随机移动创建一个计时器
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [&]() { this->randomMove(); });

    btnAutoMv = new QPushButton(this->parentWindow);
    btnAutoMv->setGeometry(30, 260, 100, 30);
    btnAutoMv->setText("AutoMv");
    btnAutoMv->setCursor(Qt::PointingHandCursor);
    connect(this->btnAutoMv, &QPushButton::clicked, this, &ChessBoard::on_btnAutoMv_clicked);

    btnStopAutoMv = new QPushButton(this->parentWindow);
    btnStopAutoMv->setGeometry(30, 310, 100, 30);
    btnStopAutoMv->setText("StopAutoMv");
    btnStopAutoMv->setCursor(Qt::PointingHandCursor);
    connect(this->btnStopAutoMv, &QPushButton::clicked, this, &ChessBoard::on_btnStopAutoMv_clicked);
}

ChessBoard::~ChessBoard() {
    for (auto obj : this->players) {
        delete obj;
    }
    delete hintPlayer;
    delete labelInfo;
    delete this->btnStopAutoMv;
    delete this->btnAutoMv;
    delete this->btnRandomMove;
}

void ChessBoard::setActivatedPlayer(Player* _activatedPlayer) {
    // 无效化上一个玩家
    if (activatedPlayer != nullptr) {
        activatedPlayer->setActivated(false);
    }
    activatedPlayer = _activatedPlayer;
    activatedPlayer->setActivated(true);
}

bool ChessBoard::checkMove(ChessPosition p1, ChessPosition p2)
{
    hintPlayer->clear();
    selectedChess = this->activatedPlayer->getChess(p1);
    if(!selectedChess){
        return false;
    }
    getHint();
    selectedChess=nullptr;
    return hintPlayer->getChess(p2);
}

bool ChessBoard::moveA2B(ChessPosition p1, ChessPosition p2)
{
    hintPlayer->clear();
    selectedChess = this->activatedPlayer->getChess(p1);
    if(!selectedChess){
        return false;
    }
    getHint();
    Marble* dest=hintPlayer->getChess(p2);
    if(!dest){
        selectedChess=nullptr;
        return false;
    }
    moveChess(dest);
    return true;
}

void ChessBoard::setNextActivatedPlayer() {
    // 跳过已经完赛的玩家
    do{
        ++activatedPlayerID;
        if (activatedPlayerID >= playerNum)
            activatedPlayerID = 0;
    }while(this->players[activatedPlayerID]->flag&4);
    setActivatedPlayer(this->players[activatedPlayerID]);
}

void ChessBoard::getHint() {
    if(this->god){
        for(int i=0;i<121;i++){
            if(!getChess(board::pst[i*2],board::pst[i*2+1])){
                this->hintPlayer->chesses.push_back(new Marble(this->parentWindow, board::pst[i*2],board::pst[i*2+1], color::hint));
                this->hintPlayer->chesses.back()->addTo(hintPlayer);
                this->hintPlayer->chesses.back()->setCursor(Qt::PointingHandCursor);
                this->hintPlayer->chesses.back()->from = this->selectedChess;
            }
        }
    }
    else{
        std::queue<Marble*> Q;
        bool vis[2 * board::indexBoundary + 1][2 * board::indexBoundary + 1];
        memset(vis, 0, sizeof(vis));
        Q.push(this->selectedChess);
        // 相邻跳跃
        while (!Q.empty()) {
            Marble* u = Q.front();
            Q.pop();
            for (auto player : this->players) {
                for (auto chess : player->chesses) {
                    ChessPosition midPst = chess->chessPosition;
                    // 1.共线 2.mid 和 u 不是同一个棋子 3.相邻
                    if (isCollinear(u->chessPosition, midPst) && midPst != this->selectedChess->chessPosition && isNeighbor(u->chessPosition, midPst)) {
                        ChessPosition dest = jumpOver(u->chessPosition, midPst);
                        int dDestX = dest.first + board::indexBoundary, dDestY = dest.second + board::indexBoundary;
                        //                    qDebug()<<"--------------bg";
                        //                    outChessPostion(u->chessPosition);
                        //                    outChessPostion(dest);
                        //                    qDebug()<<isAnyChessBetween(this,u->chessPosition,midPst,dest);
                        //                    qDebug()<<"--------------ed";
                        // 1.没有棋子在中间 2.宽搜没搜过 3.没有棋子占用 4.在边界内
                        if (!isAnyChessBetween(this, u->chessPosition, midPst, dest) && !vis[dDestX][dDestY] && !occupiedPst[dDestX][dDestY] && isWithinBoundary(dest)) {
                            vis[dDestX][dDestY] = true;
                            this->hintPlayer->chesses.push_back(new Marble(this->parentWindow, dest.first, dest.second, color::hint));
                            this->hintPlayer->chesses.back()->addTo(hintPlayer);
                            this->hintPlayer->chesses.back()->setCursor(Qt::PointingHandCursor);
                            this->hintPlayer->chesses.back()->from = u;
                            Q.push(this->hintPlayer->chesses.back());
                            // this->hintPlayer->chesses.back()->show();
                            this->hintPlayer->chessNum++;
                        }
                    }
                }
            }
        }
        // 相邻移动
        for (int k = 0; k < 6; k++) {
            ChessPosition dest(this->selectedChess->chessX + board::dx[k], this->selectedChess->chessY + board::dy[k]);
            int dDestX = dest.first + board::indexBoundary, dDestY = dest.second + board::indexBoundary;
            //        outChessPostion(this->selectedChess->chessPosition);
            //        outChessPostion(dest);
            if (!vis[dDestX][dDestY] && !occupiedPst[dDestX][dDestY] && isWithinBoundary(dest)) {
                vis[dDestX][dDestY] = true;
                this->hintPlayer->chesses.push_back(new Marble(this->parentWindow, dest.first, dest.second, color::hint));
                this->hintPlayer->chesses.back()->addTo(hintPlayer);
                this->hintPlayer->chesses.back()->setCursor(Qt::PointingHandCursor);
                this->hintPlayer->chesses.back()->from = this->selectedChess;
                Q.push(this->hintPlayer->chesses.back());
                //            this->hintPlayer->chesses.back()->show();
                this->hintPlayer->chessNum++;
            }
        }
    }

    this->hintPlayer->chessNum = this->hintPlayer->chesses.size();

    // 必须添加完 hint->chesses 再设置，因为这个函数还完成了修改鼠标手势的工作，必须保证棋子已经创建好
    this->hintPlayer->setActivated(true);
    //    this->activatedPlayer->setActivated(false);
}

void ChessBoard::showHint() {
    for (auto chess : hintPlayer->chesses) {
        chess->setHidden(false);
        // 小知识：不在构造函数中创建的控件是不会自动显示的，必须显式调用show()
        chess->show();
    }
}

void ChessBoard::unshowHint() {
    for (auto chess : hintPlayer->chesses) {
        chess->setHidden(true);
    }
}

void ChessBoard::moveChess(Marble* dest) {
    unshowHint();
    this->stepNum++;
    selectedChess->moveToWithPath(dest);
    selectedChess = nullptr;
    nextTurn();
    updateLabelInfo();
}

void ChessBoard::chooseChess(Marble* chess) {
    hintPlayer->clear();
    if (selectedChess) {
        selectedChess = nullptr;
        return;
    }
    selectedChess = chess;
    getHint();
    showHint();
}

void ChessBoard::nextTurn() {
    if(!(activatedPlayer->flag&4) && activatedPlayer->checkWin()){
        activatedPlayer->flag=4;
        qDebug()<<activatedPlayer->name<<" wins.\n";
        this->winnerRank.push_back(this->activatedPlayer);
    }
    hintPlayer->clear();
    if(int(this->winnerRank.size())==this->playerNum){
        this->activatedPlayer->setActivated(false);
        qDebug()<<"游戏结束";
    }
    else{
        this->setNextActivatedPlayer();
    }
}

void ChessBoard::randomMove() {
    do {
        hintPlayer->clear();
        selectedChess = activatedPlayer->chesses[rand() % activatedPlayer->chessNum];
        getHint();
    } while (hintPlayer->chesses.empty());
    moveChess(hintPlayer->chesses[rand() % hintPlayer->chessNum]);
    //    ChessPostion pst=hintPlayer->chesses[rand()%hintPlayer->chessNum]->chessPosition;
    //    moveChess(pst);
}

void ChessBoard::updateLabelInfo() {
    labelInfo->setText(QString("当前行棋方为 ") + getColorName(activatedPlayer->color) + QString("\n已走步数 ") + QString::number(this->stepNum));
}

void ChessBoard::show() {
    labelInfo->show();
    for (auto player : players) {
        for (auto chess : player->chesses) {
            chess->show();
        }
    }
    btnAutoMv->show();
    btnStopAutoMv->show();
    btnRandomMove->show();
}

Marble *ChessBoard::getChess(ChessPosition p, int playerID)
{
    if(playerID==-1){
        for(Player* player:this->players){
            Marble* ret=player->getChess(p);
            if(ret){
                return ret;
            }
        }
        return nullptr;
    }
    else{
        return this->players[playerID]->getChess(p);
    }
}

Marble *ChessBoard::getChess(int x, int y, int playerID)
{
    return this->getChess(ChessPosition(x,y),playerID);
}

bool isAnyChessBetween(ChessBoard* chessBoard, ChessPosition u, ChessPosition mid, ChessPosition v) {
    for (auto player : chessBoard->players) {
        for (auto chess : player->chesses) {
            if (u != chess->chessPosition && v != chess->chessPosition && mid != chess->chessPosition && isCollinear(u, chess->chessPosition, v)) {
                return true;
            }
        }
    }
    return false;
}

void ChessBoard::on_btnRandomMove_clicked() {
    this->randomMove();
}

void ChessBoard::on_btnAutoMv_clicked() {
    // 每200ms移动一次
    this->timer->start(200);
}

void ChessBoard::on_btnStopAutoMv_clicked() {
    this->timer->stop();
}
