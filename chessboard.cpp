#include "chessboard.h"
#include "player.h"
#include "marble.h"
#include "settings.h"
#include "widget.h"
#include "util.h"
#include <queue>

ChessBoard::ChessBoard(Widget *_parentWindow, int _player_num):parentWindow(_parentWindow),playerNum(_player_num),selectedChess(0)
{
    memset(this->occupiedPst,0,sizeof(this->occupiedPst));
    for(int i=0;i<playerNum;i++){
        player.push_back(new Player(board::playerSpawn[playerNum][i],board::playerSpawn[playerNum][i],board::playerTarget[playerNum][i]));
        player.back()->addTo(this);
    }
    labelActivated=new QLabel(this->parentWindow);
    labelActivated->setGeometry(20,10,180,40);
    labelActivated->setFont(QFont("华光中圆_CNKI",16));
    setActivatedPlayer(this->player[0]);
    selectedChess=player[0]->chess[9];
    player[0]->chess[9]->setText("FUCK");
    getHint();
}

ChessBoard::~ChessBoard()
{
    for(auto obj:this->player){
        delete obj;
    }
}

void ChessBoard::setActivatedPlayer(Player *_activatedPlayer)
{
    if(activatedPlayer!=NULL){
        activatedPlayer->setActivated(false);
    }
    activatedPlayer=_activatedPlayer;
    activatedPlayer->setActivated(true);
    labelActivated->setText(QString("当前走棋方为 ")+getColorName(activatedPlayer->color));
}

void ChessBoard::getHint()
{
    for(auto obj:this->hint){
        delete obj;
    }
    std::queue<ChessPostion> Q;
    bool vis[2*board::indexBoundary+1][2*board::indexBoundary+1];
    memset(vis,0,sizeof(vis));
    Q.push(this->selectedChess->chessPosition);
    while(!Q.empty()){
        ChessPostion u=Q.front();
        Q.pop();
        for(auto pla:this->player){
            for(auto ches:pla->chess){
                ChessPostion mid=ches->chessPosition;
                if(isCollinear(u,mid) && mid!=this->selectedChess->chessPosition){
                    ChessPostion dest=jumpOver(u,mid);
                    int dDestX=dest.first+board::indexBoundary,dDestY=dest.second+board::indexBoundary;
                    // 宽搜没搜过，没有棋子占用，在边界内
                    if(!isAnyChessBetween(this,u,dest) && !vis[dDestX][dDestY] && !occupiedPst[dDestX][dDestY] && isWithinBoundary(dest)){
//                        outChessPostion(u);
//                        outChessPostion(dest);
                        vis[dDestX][dDestY]=true;
                        Q.push(dest);
                        this->hint.push_back(new Marble(this->parentWindow,dest.first,dest.second,color::hint));
                        this->hint.back()->moveTo();
                        this->hint.back()->setCursor(Qt::PointingHandCursor);
                    }
                }
            }
        }
    }
    for(int k=0;k<6;k++){
        ChessPostion dest(this->selectedChess->chessX+board::dx[k],this->selectedChess->chessY+board::dy[k]);
        int dDestX=dest.first+board::indexBoundary,dDestY=dest.second+board::indexBoundary;
        if(!vis[dDestX][dDestY] && !occupiedPst[dDestX][dDestY] && isWithinBoundary(dest)){
            vis[dDestX][dDestY]=true;
            Q.push(dest);
            this->hint.push_back(new Marble(this->parentWindow,dest.first,dest.second,color::hint));
            this->hint.back()->moveTo();
            this->hint.back()->setCursor(Qt::PointingHandCursor);
        }
    }
    for(auto ches:this->activatedPlayer->chess){
        ches->setCursor(Qt::ArrowCursor);
    }
}

bool isAnyChessBetween(ChessBoard *chessBoard, ChessPostion u, ChessPostion v)
{
    for(auto pla:chessBoard->player){
        for(auto ches:pla->chess){
            if(isCollinear(u,ches->chessPosition,v)){
                return true;
            }
        }
    }
    return false;
}
