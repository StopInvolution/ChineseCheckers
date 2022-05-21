#include "chessboard.h"
#include <ctime>
#include <queue>
#include <random>
#include <stack>
#include <iostream>
#include <QDialog>
#include <QMessageBox>
#include "networkdata.h"
#include "marble.h"
#include "player.h"
#include "settings.h"
#include "util.h"
#include "widget.h"
#include "networkUtil.h"
#include "agent.h"
#include <QDebug>
#include <algorithm>
#include "clientwidget.h"

ChessBoard::ChessBoard(Widget* _parentWindow, int _player_num,QVector<pss>* playerInfo, std::map<QString,bool>* localFlag,NetworkSocket* _socket)
    : parentWindow(_parentWindow), socket(_socket),rotateAngle(0),playerNum(_player_num),stepNum(0), clockT(30),god(false), serverPermission(true), gameResult(""), activatedPlayerID(0),activatedPlayer(nullptr),selectedChess(nullptr) {
    if(playerInfo) initPlayerInfo=*playerInfo;
    if(localFlag) initLocalFlag=*localFlag;

    srand(time(0));
    terminal = new ClientWidget(nullptr,this);
    terminal->hide();
    if(playerInfo && localFlag)
        qDebug()<< (*playerInfo)<<"  "<<(*localFlag);
    memset(this->occupiedPst, 0, sizeof(this->occupiedPst));
    if(playerInfo && localFlag){
        std::sort(playerInfo->begin(),playerInfo->end(),[](const pss& rhs1, const pss& rhs2){return rhs1.second.compare(rhs2.second)<0;});
        for(int i=0;i<playerNum;i++){
            QString name=(*playerInfo)[i].first;
            if((*localFlag)[name]){
                QString ID=(*playerInfo)[i].second;
                int spawn=getSpawn(ID);
                this->rotateAngle=spawn-4;
                break;
            }
        }
    }

    const int geo[]={0,0,530,110,680,240,600,455,330,485,180,355,260,140};
    for(int i=1;i<=6;i++){
        int j=mod6Add(i,this->rotateAngle);
        labelPlayer[i] = new ClickableQLabel(this->parentWindow);
        labelPlayer[i]->setGeometry(geo[i*2],geo[i*2+1],80,40);
        labelPlayer[i]->setFont(QFont("华光中圆_CNKI", 14));
        labelPlayer[i]->setStyleSheet(getQColor(j));
        labelPlayer[i]->setText(QString::number(i));
        labelPlayer[i]->setVisible(false);
        labelPlayer[i]->setCursor(Qt::PointingHandCursor);
    }
    labelPlayer[1]->setAlignment(Qt::AlignLeft);
    labelPlayer[2]->setAlignment(Qt::AlignLeft);
    labelPlayer[3]->setAlignment(Qt::AlignHCenter);
    labelPlayer[4]->setAlignment(Qt::AlignRight);
    labelPlayer[5]->setAlignment(Qt::AlignRight);
    labelPlayer[6]->setAlignment(Qt::AlignHCenter);

    for (int i = 0; i < playerNum; i++) {
        if(playerInfo && localFlag){
            qDebug()<<playerInfo<<" "<<localFlag;
            QString name=(*playerInfo)[i].first;
            QString ID=(*playerInfo)[i].second;
            int spawn=getSpawn(ID);
            players.push_back(new Player(spawn, spawn, getTarget(ID),((*localFlag)[name])<<1,name));
            labelPlayer[mod6Add(spawn,-this->rotateAngle)]->setText(name);
            labelPlayer[mod6Add(spawn,-this->rotateAngle)]->setVisible(true);
            connect(this->labelPlayer[mod6Add(spawn,-this->rotateAngle)],&ClickableQLabel::clicked,this->players.back(),&Player::replay);
        }
        else{
            players.push_back(new Player(board::playerSpawn[playerNum][i], board::playerSpawn[playerNum][i], board::playerTarget[playerNum][i]));
        }
        players.back()->addTo(this);
    }

    hintPlayer = new Player(color::hint, -1, -1, 2, "hint",0);
    hintPlayer->addTo(this);

    // 标签
    labelInfo = new QLabel(this->parentWindow);
    labelInfo->setGeometry(20, 10, 350, 160);
    labelInfo->setFont(QFont("华光中圆_CNKI", 14));

    if(socket){
        serverPermission=false;
    }
    updateLabelInfo();

    // START_TURN_OP 应该等信号来了再设置，但原则上其实不需要，因为开始游戏和第一个人开始下棋的信号应当会一起发送，但是接口我就放这了
    nextTurn();

    // 创建三个随机移动相关按钮
    btnRandomMove = new QPushButton(this->parentWindow);
    btnRandomMove->setGeometry(30, 460, 100, 30);
    btnRandomMove->setText("RandomMove");
    btnRandomMove->setCursor(Qt::PointingHandCursor);
    QObject::connect(this->btnRandomMove, &QPushButton::clicked,this, &ChessBoard::on_btnRandomMove_clicked);

    // 给随机移动创建一个计时器
    timer = new QTimer();
//    connect(timer, &QTimer::timeout,  this, [&]() { this->randomMove(); });
    connect(timer, &QTimer::timeout,  this, [&]() {
        emit this->btnAIMv->click();
    });

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

    btnAIMv = new QPushButton(this->parentWindow);
    btnAIMv->setGeometry(30, 210, 100, 30);
    btnAIMv->setText("AIMv");
    btnAIMv->setCursor(Qt::PointingHandCursor);
    connect(this->btnAIMv, &QPushButton::clicked, this,[&](){
        if(serverPermission){
            pcc ret=calculate(this->AIDataProducer());
            this->moveA2B(ret.first,ret.second);
        }
    });

    btnTerminal = new QPushButton(this->parentWindow);
    btnTerminal->setGeometry(760,590,30,30);
    btnTerminal->setText("~");
    btnTerminal->setCursor(Qt::PointingHandCursor);
    btnTerminal->setFlat(true);
    connect(this->btnTerminal,&QPushButton::clicked,this,&ChessBoard::onTerminal);

    connect(this->timeoutTimer, &QTimer::timeout,this,  [&](){
        resTime-=clockT/1000.0;
        if(resTime<=0){
            resTime=0;
            this->timeoutTimer->stop();
            outer.push_back(this->activatedPlayer);
            if(!socket){
                timeout();
            }
            emit overtime(getActID());
        } updateLabelInfo();});
    if(socket)
        connect(this->socket,&NetworkSocket::receive,this,&ChessBoard::nertworkProcess);

    connect(this,&ChessBoard::endgame,this,&ChessBoard::on_btnStopAutoMv_clicked);
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
    for(int i=1;i<=6;i++)
        if(this->labelPlayer[i]){
            delete this->labelPlayer[i];
        }
    delete this->btnAIMv;
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

bool ChessBoard::moveA2BWithPath(QVector<ChessPosition> *path,bool ck)
{
    if(ck) selectedChess = this->activatedPlayer->getChess(path->front());
    else selectedChess = this->getChess(path->front());
    if(!selectedChess){
        return false;
    }
    if(path->size()==2 && isCollinear(path->front(),path->back()) && isNeighbor(path->front(),path->back())){
        int vx=path->back().first,vy=path->back().second;
        if(!occupiedPst[vx+board::indexBoundary][vy+board::indexBoundary]){
            moveChess(nullptr,path);
            return true;
        }
        return false;
    }
    int n=path->size();
    for(int i=1;i<n;i++){
        ChessPosition &u=(*path)[i-1], &v=(*path)[i];
        int vx=v.first,vy=v.second;
        if(!isWithinBoundary((*path)[i]) || occupiedPst[vx+board::indexBoundary][vy+board::indexBoundary] || !isCollinear(u,v)){
                selectedChess=nullptr;
                return false;
        }
        bool flag=false;
        for(auto player:this->players){
            for(auto chess:player->chesses){
                if(isCollinear(u,chess->chessPosition,v) && isNeighbor(u,chess->chessPosition) && isNeighbor(v,chess->chessPosition)){
                    flag=true;
                    goto checkdone;
                }
            }
        }
        checkdone:
        if(!flag){
            selectedChess=nullptr;
            return false;
        }
    }
    moveChess(nullptr,path);
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
                        //                    outChessPosition(u->chessPosition);
                        //                    outChessPosition(dest);
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
            //        outChessPosition(this->selectedChess->chessPosition);
            //        outChessPosition(dest);
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

QString ChessBoard::getActID()
{
    return getID(this->activatedPlayer->spawn);
}

void ChessBoard::moveChess(Marble* dest,QVector<ChessPosition> *path) {
    // 获取路径，按要求格式保存在 s 中
    if(selectedChess==nullptr) selectedChess=this->activatedPlayer->getChess(path->front());
    // 传统点击
    if(dest){
        QString s;
        std::stack<ChessPosition> S;
        Marble* now = dest;
        while (now != selectedChess) {
            S.push(now->chessPosition);
            now = now->from;
        }
        S.push(selectedChess->chessPosition);
        while(!S.empty()){
            int x=S.top().first,y=S.top().second;
            S.pop();
            s=s+QString::number(x)+" "+QString::number(y)+" ";
        }
        s = s.left(s.size()-1);
        this->activatedPlayer->lstMove=loadChessPosition(s);
        // 本地点击，即服务器上不执行下列操作
        if(socket){
            qDebug()<<"ChessBoard 客户端尝试发送 MOVE_OP";
            socket->send(NetworkData(OPCODE::MOVE_OP,getID(this->activatedPlayer->spawn),s));
            this->serverPermission=false;
        }
    }
    // 服务器点击
    if(path){
        this->activatedPlayer->lstMove=*path;
    }

    unshowHint();
    this->stepNum++;
    selectedChess->moveToWithPath(dest,path);
    selectedChess = nullptr;
    updateLabelInfo();

    nextTurn();
}

void ChessBoard::timeout()
{
    if(!activatedPlayer){
        qDebug()<<"超时判负炸了";
        return ;
    }
    activatedPlayer->flag=5;
    activatedPlayer->clear();
    selectedChess=nullptr;

    nextTurn();
}

void ChessBoard::showRank(QString data)
{
    QStringList rk = data.split(" ");
    QString output="";
    for(int i=0;i<rk.size();i++){
        if(i>0) output+="\n";
        output+="No."+QString::number(i+1)+": "+getPlayer(rk[i])->name;
    }
    this->parentWindow->setWindowTitle("已与服务器断开，请关闭此窗口");
    gameResult=output;
    QMessageBox::about(this->parentWindow,"排行榜",output);
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
    resTime = 15;
    this->timeoutTimer->start(clockT);
    if(!activatedPlayer){
        activatedPlayerID=0;
        activatedPlayer=players.front();
        activatedPlayer->setActivated(true);
        updateLabelInfo();
        return ;
    }
    hintPlayer->clear();

    if(!(activatedPlayer->flag&4) && activatedPlayer->checkWin()){
        activatedPlayer->flag=4;
        qDebug()<<activatedPlayer->name<<" wins.\n";
        emit victory(this->activatedPlayer->name);
        this->winnerRank.push_back(this->activatedPlayer);
    }

    this->setNextActivatedPlayer();

    if(static_cast<int>(this->winnerRank.size()+this->outer.size())==this->playerNum-1){
        activatedPlayer->flag=4;
        this->activatedPlayer->setActivated(false);
        qDebug()<<activatedPlayer->name<<" wins.\n";
        emit victory(this->activatedPlayer->name);

        qDebug()<<"游戏结束";

        QString data;
        this->winnerRank.push_back(this->activatedPlayer);
        std::copy(outer.begin(),outer.end(),std::inserter(winnerRank,winnerRank.begin()));
        for(int i=0;i<playerNum;i++){
            if(i>0) data+=" ";
            data+=getID(this->winnerRank[i]->spawn);
        }
        emit endgame(data);
        this->timeoutTimer->stop();
        if(!socket){
            this->showRank(data);
        }
    }

    updateLabelInfo();
}

void ChessBoard::randomMove() {

    if(serverPermission){
        do {
            hintPlayer->clear();
            selectedChess = activatedPlayer->chesses[rand() % activatedPlayer->chessNum];
            getHint();
        } while (hintPlayer->chesses.empty());
        moveChess(hintPlayer->chesses[rand() % hintPlayer->chessNum]);
    }
}

void ChessBoard::updateLabelInfo() {
    if(gameResult.length()){
        labelInfo->setText(gameResult);
        return ;
    }
    QString waitInfo="";
    if(activatedPlayer){
        if(this->socket){
            if(activatedPlayer->flag==3){
                if(serverPermission){
                    waitInfo="你的轮次，服务端正在等待你下棋\n";
                }
                else{
                    waitInfo="你的轮次，正在等待服务端允许你下棋\n";
                }
            }
           else{
                waitInfo="你的轮次，正在等待你下棋\n";
            }
        }
    }
    else{
        waitInfo="不是你的轮次，请等待\n";
    }
    labelInfo->setText(tr("当前行棋方为\n") +
                       (this->activatedPlayer?getColorName(activatedPlayer->color)+":"+this->activatedPlayer->name:"None") +"\n"+
                       waitInfo+
                       (this->resTime>0.0?tr("剩余思考时间 ")+QString::number(this->resTime)+tr("s"):tr("思考时间超时"))+
                       tr("\n已走步数 ") + QString::number(this->stepNum));
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
    btnAIMv->show();
    btnTerminal->show();
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

Player *ChessBoard::getPlayer(QString ID)
{
    for(int i=0;i<this->playerNum;i++){
        if(getSpawn(ID)==this->players[i]->spawn){
            return this->players[i];
        }
    }
    return nullptr;
}

void ChessBoard::nertworkProcess(NetworkData data)
{
    OPCODE &op=data.op;
    QString &data1=data.data1,&data2=data.data2;
    switch(op){
    case OPCODE::MOVE_OP:{
        checkAct(data1);
        if(data2=="-1"){
            this->timeout();
        }
        else{
            QVector<ChessPosition> v;
            loadChessPosition(v,data2);
            this->moveChess(nullptr,&v);
        }
        break;
    }
    case OPCODE::START_TURN_OP:{
        serverPermission=true;
        updateLabelInfo();
        break;
    }
    case OPCODE::END_TURN_OP:{
        qDebug()<<"你确实赢了，服务器知道了";
        break;
    }
    case OPCODE::END_GAME_OP:{
        showRank(data1);
        break;
    }
    default:
        qDebug()<<"棋盘收到了无用信息，抛弃";
    }
}

QVector<AlgoPlayer> ChessBoard::AIDataProducer(){
    int i=this->activatedPlayerID;
    QVector<AlgoPlayer> vec;
    do{
        vec.push_back(this->players[i]->toAlgoPlayer());
        do{
            i++;
            if(i>=playerNum) i=0;
        }while(this->players[i]->flag&4);
    }while(i!=this->activatedPlayerID);
    return vec;
}

int ChessBoard::serverMoveProcess(QString data1,QString data2)
{
    qDebug()<<getID(this->activatedPlayer->spawn);
    if(getID(this->activatedPlayer->spawn)!=data1) return -1;
    QVector<ChessPosition> vec;
    loadChessPosition(vec,data2);
    return moveA2BWithPath(&vec);
}

bool ChessBoard::checkAct(QString ID)
{
    if(ID!="" && ID!=getID(this->activatedPlayer->spawn)){
        qDebug()<<"activatedPlayer 不一致，"<<"本地为"<<getID(this->activatedPlayer->spawn)<<" 服务端为"<<ID;
        return false;
    }
    return true;
}

void ChessBoard::onTerminal()
{
    if(this->terminal->isVisible()){
        this->terminal->hide();
    }
    else{
        this->terminal->show();
    }
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
