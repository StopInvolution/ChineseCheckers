#include "chessboard.h"
#include "player.h"
#include "marble.h"
#include "settings.h"
#include "widget.h"
#include "util.h"
#include <random>
#include <ctime>
#include <queue>

ChessBoard::ChessBoard(Widget *_parentWindow, int _player_num):parentWindow(_parentWindow),playerNum(_player_num),stepNum(0),selectedChess(0)
{
    srand(time(0));
    memset(this->occupiedPst,0,sizeof(this->occupiedPst));
    for(int i=0;i<playerNum;i++){
        players.push_back(new Player(board::playerSpawn[playerNum][i],board::playerSpawn[playerNum][i],board::playerTarget[playerNum][i]));
        players.back()->addTo(this);
    }
    hintPlayer = new Player(color::hint,-1,-1,"hint",0);
    hintPlayer->addTo(this);
    labelInfo=new QLabel(this->parentWindow);
    labelInfo->setGeometry(10,10,200,80);
    labelInfo->setFont(QFont("华光中圆_CNKI",16));
    setActivatedPlayer(this->players.front());
    this->activatedPlayerID=0;
    updateLabelInfo();
}

ChessBoard::~ChessBoard()
{
    for(auto obj:this->players){
        delete obj;
    }
    delete hintPlayer;
    delete labelInfo;
}

void ChessBoard::setActivatedPlayer(Player *_activatedPlayer)
{
    if(activatedPlayer!=NULL){
        activatedPlayer->setActivated(false);
    }
    activatedPlayer=_activatedPlayer;
    activatedPlayer->setActivated(true);
}

void ChessBoard::setNextActivatedPlayer()
{
    activatedPlayerID++;
    if(activatedPlayerID>=playerNum) activatedPlayerID=0;
    setActivatedPlayer(this->players[activatedPlayerID]);
}

void ChessBoard::getHint()
{
    std::queue<ChessPostion> Q;
    bool vis[2*board::indexBoundary+1][2*board::indexBoundary+1];
    memset(vis,0,sizeof(vis));
    Q.push(this->selectedChess->chessPosition);
    while(!Q.empty()){
        ChessPostion u=Q.front();
        Q.pop();
        for(auto player:this->players){
            for(auto chess:player->chesses){
                ChessPostion mid=chess->chessPosition;
                if(isCollinear(u,mid) && mid!=this->selectedChess->chessPosition){
                    ChessPostion dest=jumpOver(u,mid);
                    int dDestX=dest.first+board::indexBoundary,dDestY=dest.second+board::indexBoundary;
//                    qDebug()<<"--------------bg";
//                    outChessPostion(u);
//                    outChessPostion(dest);
//                    qDebug()<<isAnyChessBetween(this,u,mid,dest);
//                    qDebug()<<"--------------ed";
                    // 没有棋子在中间，宽搜没搜过，没有棋子占用，在边界内
                    if(!isAnyChessBetween(this,u,mid,dest) && !vis[dDestX][dDestY] && !occupiedPst[dDestX][dDestY] && isWithinBoundary(dest)){
                        vis[dDestX][dDestY]=true;
                        Q.push(dest);
                        this->hintPlayer->chesses.push_back(new Marble(this->parentWindow,dest.first,dest.second,color::hint));
                        this->hintPlayer->chesses.back()->addTo(hintPlayer);
                        this->hintPlayer->chesses.back()->setCursor(Qt::PointingHandCursor);
//                        this->hintPlayer->chesses.back()->show();
                        this->hintPlayer->chessNum++;
                    }
                }
            }
        }
    }
    for(int k=0;k<6;k++){
        ChessPostion dest(this->selectedChess->chessX+board::dx[k],this->selectedChess->chessY+board::dy[k]);
        int dDestX=dest.first+board::indexBoundary,dDestY=dest.second+board::indexBoundary;
//        outChessPostion(this->selectedChess->chessPosition);
//        outChessPostion(dest);
        if(!vis[dDestX][dDestY] && !occupiedPst[dDestX][dDestY] && isWithinBoundary(dest)){
            vis[dDestX][dDestY]=true;
            Q.push(dest);
            this->hintPlayer->chesses.push_back(new Marble(this->parentWindow,dest.first,dest.second,color::hint));
            this->hintPlayer->chesses.back()->addTo(hintPlayer);
            this->hintPlayer->chesses.back()->setCursor(Qt::PointingHandCursor);
//            this->hintPlayer->chesses.back()->show();
            this->hintPlayer->chessNum++;
        }
    }
    this->hintPlayer->chessNum=this->hintPlayer->chesses.size();

    // 添加完 hint->chesses 再设置
    this->hintPlayer->setActivated(true);
    //    this->activatedPlayer->setActivated(false);
}

void ChessBoard::showHint()
{
    for(auto chess:hintPlayer->chesses){
        chess->show();
    }
}

void ChessBoard::moveChess(ChessPostion pst)
{
    this->stepNum++;
    selectedChess->moveTo(pst);
    nextTurn();
    updateLabelInfo();
}

void ChessBoard::chooseChess(Marble *chess)
{
    hintPlayer->clear();
    selectedChess=chess;
    getHint();
    showHint();
}

void ChessBoard::nextTurn()
{
    hintPlayer->clear();
    this->setNextActivatedPlayer();
}

void ChessBoard::randomMove()
{
    do{
        hintPlayer->clear();
        selectedChess=activatedPlayer->chesses[rand()%activatedPlayer->chessNum];
        getHint();
    }while(hintPlayer->chesses.empty());
    moveChess(hintPlayer->chesses[rand()%hintPlayer->chessNum]->chessPosition);
//    ChessPostion pst=hintPlayer->chesses[rand()%hintPlayer->chessNum]->chessPosition;
//    moveChess(pst);
}

void ChessBoard::updateLabelInfo()
{
//    labelInfo->setText("213");
//    qDebug()<<activatedPlayer;
//    qDebug()<<getColorName(activatedPlayer->color);
//    qDebug()<<activatedPlayer->color;
    labelInfo->setText(QString("当前行棋方为 ")+getColorName(activatedPlayer->color)+QString("\n已走步数 ")+QString::number(this->stepNum));
}

bool isAnyChessBetween(ChessBoard *chessBoard, ChessPostion u, ChessPostion mid, ChessPostion v)
{
    for(auto player:chessBoard->players){
        for(auto chess:player->chesses){
            if(u!=chess->chessPosition && v!=chess->chessPosition && mid!=chess->chessPosition && isCollinear(u,chess->chessPosition,v)){
                return true;
            }
        }
    }
    return false;
}
