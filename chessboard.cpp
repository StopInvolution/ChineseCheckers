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
#include <QDateTime>
#include "clientwidget.h"

ChessBoard::ChessBoard(Widget* _parentWindow, int _player_num,QVector<pss>* playerInfo, std::map<QString,bool>* localFlag)
    : parentWindow(_parentWindow),rotateAngle(0),playerNum(_player_num),stepNum(0),clockT(30),initResTime(Network::resTime),initAgentCD(100),god(false), movePermission(true),onAgent(false), onSingleAgent(false), gameResult(""), activatedPlayerID(0),activatedPlayer(nullptr),selectedChess(nullptr) {
    if(playerInfo) initPlayerInfo=*playerInfo;
    if(localFlag) initLocalFlag=*localFlag;

    srand(time(0));
    console = new ClientWidget(nullptr,this);
    console->hide();
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

    // 标签
    labelInfo = new QLabel(this->parentWindow);
    labelInfo->setGeometry(20, 10, 350, 160);
    labelInfo->setFont(QFont("华光中圆_CNKI", 14));

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
        labelPlayer[i]->setAlignment(Qt::AlignVCenter);
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
            
            players.back()->agent_algorithm=get_agent_algorithm("auto");
            
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

    updateLabelInfo();

    // START_TURN_OP 应该等信号来了再设置，但原则上其实不需要，因为开始游戏和第一个人开始下棋的信号应当会一起发送，但是接口我就放这了
    nextTurn();

    // 创建三个随机移动相关按钮
    btnRandomMove = new QPushButton(this->parentWindow);
    btnRandomMove->setGeometry(30, 460, 100, 30);
    btnRandomMove->setText("RandomMove");
    btnRandomMove->setCursor(Qt::PointingHandCursor);
    QObject::connect(this->btnRandomMove, &QPushButton::clicked,this, &ChessBoard::on_btnRandomMove_clicked);

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
    connect(this->btnAIMv, &QPushButton::clicked, this, [&](){
        if(!this->onAgent && this->movePermission){
            this->onAgent=true;
            this->onSingleAgent=true;
            agent_move();
        }
    });

    btnConsole = new QPushButton(this->parentWindow);
    btnConsole->setGeometry(760,590,30,30);
    btnConsole->setText("~");
    btnConsole->setCursor(Qt::PointingHandCursor);
    btnConsole->setFlat(true);
    connect(this->btnConsole,&QPushButton::clicked,this,&ChessBoard::onConsole);

    connect(this->timeoutTimer, &QTimer::timeout,this,  [&](){
        resTime-=clockT/1000.0;
        if(resTime<=0){
            resTime=0;
            this->timeoutTimer->stop();
            emit overtime(getActID());
        } updateLabelInfo();});

    connect(this,&ChessBoard::overtime,this,&ChessBoard::timeout);
    connect(this,&ChessBoard::endgame,this,&ChessBoard::on_btnStopAutoMv_clicked);

    this->agentCD=clock();
    agT=new AgentThread();
    connect(agT,&AgentThread::finished,this,[&](){
        if(this->onAgent){
            this->moveA2B(agT->res.first,agT->res.second);
            this->agentCD=clock();
            if(this->onSingleAgent){
                this->onSingleAgent=false;
                this->onAgent=false;
            }
        }
        else{
            qDebug()<<"onAgent = false";
        }
    });
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
    delete this->console;
    delete this->agT;
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

void ChessBoard::agent_move()
{
    if(movePermission){
        int dt = this->agentCD+200-clock();
        dt = dt<0?0:dt;
        QTimer::singleShot(dt,this,[&](){
//            pcc ret=this->activatedPlayer->agent_algorithm(this->AIDataProducer());
            agT->set(activatedPlayer->agent_algorithm,this->AIDataProducer());
            agT->start();
        });
    }
}

Player *ChessBoard::getPlayerByName(QString name)
{
    for(Player* player:this->players){
        if(player->name==name){
            return player;
        }
    }
    return nullptr;
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

    unshowHint();
    this->stepNum++;
    selectedChess->moveToWithPath(dest,path);
    selectedChess = nullptr;
    updateLabelInfo();

    nextTurn();
}

void ChessBoard::timeout()
{
    this->outer.push_back(activatedPlayer);
    activatedPlayer->flag=5;
    activatedPlayer->clear();
    selectedChess=nullptr;

    nextTurn();
}

void ChessBoard::showRank(QString data)
{
    QStringList rk = data.split(" ");
    QString output="";
    qDebug()<<rk.size();
    for(int i=0;i<rk.size();i++){
        Player* player=getPlayerByID(rk[i]);
        QString name=player?(player->name):rk[i];
        if(i>0) output+="\n";
        output+="No."+QString::number(i+1)+": "+name;
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
    resTime = initResTime;

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
        qDebug()<<"local ChessBoard: "<<activatedPlayer->name<<" wins.\n";
        emit victory(this->activatedPlayer->name);
        this->winnerRank.push_back(this->activatedPlayer);
    }

    this->setNextActivatedPlayer();

    if(static_cast<int>(this->winnerRank.size()+this->outer.size())==this->playerNum-1){
        activatedPlayer->flag=4;
        this->activatedPlayer->setActivated(false);
        qDebug()<<activatedPlayer->name<<" wins.\n";
        emit victory(this->activatedPlayer->name);

        qDebug()<<"local ChessBoard: "<<"game end";

        QString data;
        this->winnerRank.push_back(this->activatedPlayer);
        std::copy(outer.begin(),outer.end(),std::inserter(winnerRank,winnerRank.begin()));
        for(int i=0;i<playerNum;i++){
            if(i>0) data+=" ";
            data+=getID(this->winnerRank[i]->spawn);
        }
        this->timeoutTimer->stop();
        if(this->initLocalFlag.size()>0){
            this->showRank(data);
        }
        this->labelInfo->setText("");
        emit endgame(data);
        return ;
    }

    updateLabelInfo();
    emit onMyTurn();
}

void ChessBoard::randomMove() {
    if(movePermission && !this->onAgent){
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
    QString waitInfo="不是你的轮次，请等待\n";
    if(activatedPlayer){
         waitInfo="你的轮次，正在等待你下棋\n";
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
    btnConsole->show();
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

Player *ChessBoard::getPlayerByID(QString ID)
{
    for(int i=0;i<this->playerNum;i++){
        if(getSpawn(ID)==this->players[i]->spawn){
            return this->players[i];
        }
    }
    return nullptr;
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

bool ChessBoard::checkAct(QString ID)
{
    if(ID!="" && ID!=getID(this->activatedPlayer->spawn)){
        qDebug()<<"activatedPlayer 不一致，"<<"本地为"<<getID(this->activatedPlayer->spawn)<<" 服务端为"<<ID;
        return false;
    }
    return true;
}

void ChessBoard::onConsole()
{
    if(this->console->isVisible()){
        this->console->hide();
    }
    else{
        this->console->show();
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
    if(!this->onAgent){
        if(movePermission && !this->onAgent) this->agent_move();
        this->onAgent=true;
        connect(this,&ChessBoard::onMyTurn,this,&ChessBoard::agent_move);
    }
}

void ChessBoard::on_btnStopAutoMv_clicked() {
    this->onAgent=false;
    disconnect(this,&ChessBoard::onMyTurn,this,&ChessBoard::agent_move);
}

SocketChessBoard::SocketChessBoard(Widget *_parentWindow, int _player_num, QVector<pss> *playerInfo, std::map<QString, bool> *localFlag, NetworkSocket *_socket)
    :ChessBoard(_parentWindow,_player_num,playerInfo,localFlag), socket(_socket){
    movePermission=false;
    connect(this->socket,&NetworkSocket::receive,this,&SocketChessBoard::nertworkProcess);
    disconnect(this,&ChessBoard::overtime,this,&ChessBoard::timeout);
}

SocketChessBoard::~SocketChessBoard()
{
    delete this->socket;
}

void SocketChessBoard::nextTurn() {
    resTime = initResTime;

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

        qDebug()<<"game end";

        QString data;
        this->winnerRank.push_back(this->activatedPlayer);
//        std::copy(outer.begin(),outer.end(),std::inserter(winnerRank,winnerRank.begin()));
//        for(int i=0;i<playerNum;i++){
//            if(i>0) data+=" ";
//            data+=getID(this->winnerRank[i]->spawn);
//        }
        this->timeoutTimer->stop();
        this->btnStopAutoMv->click();
        emit endgame(data);
        return ;
    }

    updateLabelInfo();
}

void SocketChessBoard::moveChess(Marble* dest,QVector<ChessPosition> *path) {
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
        qDebug()<<"客户端尝试发送 MOVE_OP "<<getID(this->activatedPlayer->spawn)<<" "<<s;
        socket->send(NetworkData(OPCODE::MOVE_OP,getID(this->activatedPlayer->spawn),s));
        this->movePermission=false;
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

void SocketChessBoard::nertworkProcess(NetworkData data)
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
        qDebug()<<data1;
        for(auto player:this->players){
            if((player->flag&2) && getID(player->spawn)==data1){
                movePermission=true;
                break;
            }
        }
//        QDateTime time1 = QDateTime::currentDateTime();   //获取当前时间
        int timeT = time(NULL),timeServer=data2.toInt();
        resTime=timeT+Network::resTime-timeServer;
        this->timeoutTimer->start(clockT);
        updateLabelInfo();
        if(movePermission){
            emit this->onMyTurn();
        }
        break;
    }
    case OPCODE::END_TURN_OP:{
        qDebug()<<"你确实赢了，服务器知道了";
        this->on_btnStopAutoMv_clicked();
        break;
    }
    case OPCODE::END_GAME_OP:{
        this->on_btnStopAutoMv_clicked();
        qDebug()<<"服务器发送终止信号"<<" "<<data1;
        this->timeoutTimer->stop();
        this->btnStopAutoMv->click();
        this->labelInfo->setText("");
        this->updateLabelInfo();
        showRank(data1);
        break;
    }
    default:
        qDebug()<<"棋盘收到了无用信息，抛弃";
    }
}

void SocketChessBoard::updateLabelInfo() {
    if(gameResult.length()){
        labelInfo->setText(gameResult);
        return ;
    }
    QString waitInfo="不是你的轮次，请等待\n";
    if(activatedPlayer){
         if(activatedPlayer->flag==3){
            if(movePermission){
                waitInfo="你的轮次，服务端正在等待你下棋\n";
            }
            else{
                waitInfo="你的轮次，正在等待服务端允许你下棋\n";
            }
        }
    }
    labelInfo->setText(tr("当前行棋方为\n") +
                       (this->activatedPlayer?getColorName(activatedPlayer->color)+":"+this->activatedPlayer->name:"None") +"\n"+
                       waitInfo+
                       (this->resTime>0.0?tr("剩余思考时间 ")+QString::number(this->resTime)+tr("s"):tr("思考时间超时"))+
                       tr("\n已走步数 ") + QString::number(this->stepNum));
}

AgentThread::AgentThread()
{

}

AgentThread::AgentThread(Agent_algorithm *_algo, QVector<AlgoPlayer> _data):
    algo(_algo),data(_data)
{
}

void AgentThread::set(Agent_algorithm _algo, QVector<AlgoPlayer> _data)
{
    algo=_algo;
    data=_data;
}

void AgentThread::run()
{
    this->res = algo(data);
    qDebug()<<"agentThread test";
}

void ServerChessBoard::moveChess(Marble *dest, QVector<ChessPosition> *path)
{
    // 获取路径，按要求格式保存在 s 中
    if(selectedChess==nullptr) selectedChess=this->activatedPlayer->getChess(path->front());
    this->activatedPlayer->lstMove=*path;
    unshowHint();
    this->stepNum++;
    selectedChess->moveToWithPath(dest,path);
    selectedChess = nullptr;
    updateLabelInfo();
}

void ServerChessBoard::timeout()
{
    activatedPlayer->flag=5;
    activatedPlayer->clear();
    selectedChess=nullptr;
}

void ServerChessBoard::nextTurn()
{
    resTime = initResTime;

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
        qDebug()<<"server ChessBoard: "<<activatedPlayer->name<<" wins.\n";
        emit victory(this->activatedPlayer->name);
        this->winnerRank.push_back(this->activatedPlayer);
    }

    this->setNextActivatedPlayer();

    if(static_cast<int>(this->winnerRank.size()+this->outer.size())==this->playerNum-1){
        activatedPlayer->flag=4;
        this->activatedPlayer->setActivated(false);
        qDebug()<<activatedPlayer->name<<" wins.\n";
        emit victory(this->activatedPlayer->name);

        qDebug()<<"server ChessBoard: "<<"game end";

        QString data;
        this->winnerRank.push_back(this->activatedPlayer);
        std::copy(outer.begin(),outer.end(),std::inserter(winnerRank,winnerRank.begin()));
        for(int i=0;i<playerNum;i++){
            if(i>0) data+=" ";
            data+=getID(this->winnerRank[i]->spawn);
        }
        this->timeoutTimer->stop();
        if(this->initLocalFlag.size()>0){
            this->showRank(data);
        }
        this->labelInfo->setText("");
        emit endgame(data);
        return ;
    }

    emit startTurn(getID(this->activatedPlayer->spawn));
    updateLabelInfo();
    emit onMyTurn();
}
