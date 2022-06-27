#include "util.h"
#include "agent.h"
#include <QDebug>
#include <QVector>
#include <cmath>
#include <queue>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>

//pcc: pair<ChessPosition, ChessPosition>
//ChessPosition: pair<int,int>
//QVector: ~vector

//AlgoPlayer: (settings.h l64)
/*class AlgoPlayer{
public:
    AlgoPlayer()=default;
    AlgoPlayer(int s){
        spawn=s;
        target=s+3>6?s-3:s+3;
        for(int i=0;i<10;i++){
            pst[i]=ChessPosition(board::spawnPst[s][i*2],board::spawnPst[s][i*2+1]);
        }
    }
    QString name; // 玩家名
    int spawn,target; // 棋子的起点和终点
    ChessPosition pst[10]; //棋子位置
};*/

//AlgoPlayer NowPlayer = vec[0];

//vec: (chessboard.cpp l534)
/*QVector<AlgoPlayer> ChessBoard::AIDataProducer(){
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
}*/

//from util.h
//bool isWithinBoundary(const ChessPosition& arg);
// 是否相邻
//bool isNeighbor(const ChessPosition& arg1, const ChessPosition& arg2);
// 返回 arg1 到 arg2 倍长线的终点
//ChessPosition jumpOver(const ChessPosition& arg1, const ChessPosition& arg2);
//alpha * pi/3 :所有棋子逆时针旋转角度
//ChessPosition rotateCounterclockwise (ChessPosition begin, int alpha)

static const int OUTOFBOUNDARY = -1;
static const int EMPTY = 0;
static const int move[6][2] = {{-1, 1},{ 0, 1},
                               {-1, 0},        { 1, 0},
                                       { 0,-1},{ 1,-1}};
static const int dist[7][17][17] = {
    {},

    {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 9,-1,-1,-1,-1}, // target = 1
     {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 9, 8,-1,-1,-1,-1},
     {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 9, 8, 7,-1,-1,-1,-1},
     {-1,-1,-1,-1,-1,-1,-1,-1,-1, 9, 8, 7, 6,-1,-1,-1,-1},
     {-1,-1,-1,-1,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
     {-1,-1,-1,-1,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2,-1},
     {-1,-1,-1,-1,13,12,11,10, 9, 8, 7, 6, 5, 4, 3,-1,-1},
     {-1,-1,-1,-1,13,12,11,10, 9, 8, 7, 6, 5, 4,-1,-1,-1},
     {-1,-1,-1,-1,13,12,11,10, 9, 8, 7, 6, 5,-1,-1,-1,-1},
     {-1,-1,-1,14,13,12,11,10, 9, 8, 7, 6, 6,-1,-1,-1,-1},
     {-1,-1,15,14,13,12,11,10, 9, 8, 7, 7, 7,-1,-1,-1,-1},
     {-1,16,15,14,13,12,11,10, 9, 8, 8, 8, 8,-1,-1,-1,-1},
     {17,16,15,14,13,12,11,10, 9, 9, 9, 9, 9,-1,-1,-1,-1},
     {-1,-1,-1,-1,13,12,11,10,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1,13,12,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1,13,12,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},

    {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,13,-1,-1,-1,-1}, //target = 2
     {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,13,12,-1,-1,-1,-1},
     {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,13,12,11,-1,-1,-1,-1},
     {-1,-1,-1,-1,-1,-1,-1,-1,-1,13,12,11,10,-1,-1,-1,-1},
     {-1,-1,-1,-1,17,16,15,14,13,12,11,10, 9, 9, 9, 9, 9},
     {-1,-1,-1,-1,16,15,14,13,12,11,10, 9, 8, 8, 8, 8,-1},
     {-1,-1,-1,-1,15,14,13,12,11,10, 9, 8, 7, 7, 7,-1,-1},
     {-1,-1,-1,-1,14,13,12,11,10, 9, 8, 7, 6, 6,-1,-1,-1},
     {-1,-1,-1,-1,13,12,11,10, 9, 8, 7, 6, 5,-1,-1,-1,-1},
     {-1,-1,-1,13,12,11,10, 9, 8, 7, 6, 5, 4,-1,-1,-1,-1},
     {-1,-1,13,12,11,10, 9, 8, 7, 6, 5, 4, 3,-1,-1,-1,-1},
     {-1,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2,-1,-1,-1,-1},
     {13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1,-1,-1,-1,-1},
     {-1,-1,-1,-1, 9, 8, 7, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1, 9, 8, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1, 9, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},

    {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,17,-1,-1,-1,-1}, //target = 3
     {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,16,16,-1,-1,-1,-1},
     {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,15,15,15,-1,-1,-1,-1},
     {-1,-1,-1,-1,-1,-1,-1,-1,-1,14,14,14,14,-1,-1,-1,-1},
     {-1,-1,-1,-1,13,13,13,13,13,13,13,13,13,13,13,13,13},
     {-1,-1,-1,-1,12,12,12,12,12,12,12,12,12,12,12,12,-1},
     {-1,-1,-1,-1,11,11,11,11,11,11,11,11,11,11,11,-1,-1},
     {-1,-1,-1,-1,10,10,10,10,10,10,10,10,10,10,-1,-1,-1},
     {-1,-1,-1,-1, 9, 9, 9, 9, 9, 9, 9, 9, 9,-1,-1,-1,-1},
     {-1,-1,-1, 9, 8, 8, 8, 8, 8, 8, 8, 8, 9,-1,-1,-1,-1},
     {-1,-1, 9, 8, 7, 7, 7, 7, 7, 7, 7, 8, 9,-1,-1,-1,-1},
     {-1, 9, 8, 7, 6, 6, 6, 6, 6, 6, 7, 8, 9,-1,-1,-1,-1},
     { 9, 8, 7, 6, 5, 5, 5, 5, 5, 6, 7, 8, 9,-1,-1,-1,-1},
     {-1,-1,-1,-1, 4, 4, 4, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1, 3, 3, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1, 2, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},

    {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,13,-1,-1,-1,-1}, //target = 4
     {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,12,13,-1,-1,-1,-1},
     {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,12,13,-1,-1,-1,-1},
     {-1,-1,-1,-1,-1,-1,-1,-1,-1,10,11,12,13,-1,-1,-1,-1},
     {-1,-1,-1,-1, 9, 9, 9, 9, 9,10,11,12,13,14,15,16,17},
     {-1,-1,-1,-1, 8, 8, 8, 8, 9,10,11,12,13,14,15,16,-1},
     {-1,-1,-1,-1, 7, 7, 7, 8, 9,10,11,12,13,14,15,-1,-1},
     {-1,-1,-1,-1, 6, 6, 7, 8, 9,10,11,12,13,14,-1,-1,-1},
     {-1,-1,-1,-1, 5, 6, 7, 8, 9,10,11,12,13,-1,-1,-1,-1},
     {-1,-1,-1, 4, 5, 6, 7, 8, 9,10,11,12,13,-1,-1,-1,-1},
     {-1,-1, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,-1,-1,-1,-1},
     {-1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,-1,-1,-1,-1},
     { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,-1,-1,-1,-1},
     {-1,-1,-1,-1, 6, 7, 8, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1, 7, 8, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1, 8, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},

    {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 9,-1,-1,-1,-1}, //target = 5
     {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 8, 9,-1,-1,-1,-1},
     {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 7, 8, 9,-1,-1,-1,-1},
     {-1,-1,-1,-1,-1,-1,-1,-1,-1, 6, 7, 8, 9,-1,-1,-1,-1},
     {-1,-1,-1,-1, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13},
     {-1,-1,-1,-1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,-1},
     {-1,-1,-1,-1, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,-1,-1},
     {-1,-1,-1,-1, 4, 5, 6, 7, 8, 9,10,11,12,13,-1,-1,-1},
     {-1,-1,-1,-1, 5, 6, 7, 8, 9,10,11,12,13,-1,-1,-1,-1},
     {-1,-1,-1, 6, 6, 7, 8, 9,10,11,12,13,14,-1,-1,-1,-1},
     {-1,-1, 7, 7, 7, 8, 9,10,11,12,13,14,15,-1,-1,-1,-1},
     {-1, 8, 8, 8, 8, 9,10,11,12,13,14,15,16,-1,-1,-1,-1},
     { 9, 9, 9, 9, 9,10,11,12,13,14,15,16,17,-1,-1,-1,-1},
     {-1,-1,-1,-1,10,11,12,13,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1,11,12,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1,12,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},

    {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1,-1,-1,-1}, //target = 6
     {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 2, 2,-1,-1,-1,-1},
     {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 3, 3, 3,-1,-1,-1,-1},
     {-1,-1,-1,-1,-1,-1,-1,-1,-1, 4, 4, 4, 4,-1,-1,-1,-1},
     {-1,-1,-1,-1, 9, 8, 7, 6, 5, 5, 5, 5, 5, 6, 7, 8, 9},
     {-1,-1,-1,-1, 9, 8, 7, 6, 6, 6, 6, 6, 6, 7, 8, 9,-1},
     {-1,-1,-1,-1, 9, 8, 7, 7, 7, 7, 7, 7, 7, 8, 9,-1,-1},
     {-1,-1,-1,-1, 9, 8, 8, 8, 8, 8, 8, 8, 8, 9,-1,-1,-1},
     {-1,-1,-1,-1, 9, 9, 9, 9, 9, 9, 9, 9, 9,-1,-1,-1,-1},
     {-1,-1,-1,10,10,10,10,10,10,10,10,10,10,-1,-1,-1,-1},
     {-1,-1,11,11,11,11,11,11,11,11,11,11,11,-1,-1,-1,-1},
     {-1,12,12,12,12,12,12,12,12,12,12,12,12,-1,-1,-1,-1},
     {13,13,13,13,13,13,13,13,13,13,13,13,13,-1,-1,-1,-1},
     {-1,-1,-1,-1,14,14,14,14,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1,15,15,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1,16,16,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
     {-1,-1,-1,-1,17,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}}
};


static const int INF = 2147483647;
static const int PRUNED = 998244353;
static const int MINNODE = -1, MAXNODE = 1;

static int MinimaxDepthLimit = 5;
static const int MINIMAXDEPTHFORPRE = 3;
static const bool IGNORERETREAT = true;
static const int TIMELIMIT = 14000; //ms

static const double RATEOFBACK[3] = {0,2,4}; //落后棋子优先

static const double POTENTIALCOUNT = 2;
static const double POTENTIALDECAY = 0.5;
static const double RATEOFPOTENTIAL[3] = {2,1,0};

static int Steps = 0;
static const int OPENINGSTEPS[7] = {0,0,20,30,0,0,0};
static const int ENDGAMESTEPS[7] = {0,0,60,90,0,0,0};
static const int OPENING = 0;
static const int MIDDLE = 1;
static const int ENDGAME = 2;

void printvec(QVector<AlgoPlayer> vec) {
    qDebug() << "printvec";
    int vsize = vec.size();
    for (int i = 0;i < vsize;i++) {
        qDebug() << vec[i].name;
        qDebug() << "spawn =" << vec[i].spawn << ", target =" << vec[i].target;
        for (int j = 0;j < 10;j++)
            qDebug() << vec[i].pst[j].first << "," << vec[i].pst[j].second;
    }
}

void printmap(int (&map)[17][17]) {
    for (int j = 16;j >= 0;j--) {
        for (int i = 0;i < 17;i++) {
            if (map[i][j] == -1) printf("   ");
            else printf("%3d",map[i][j]);
        }
        printf("\n");
    }
}

void printdist() {
    for (int j = 16;j >= 0;j--) {
        for (int i = 0;i < 17;i++) {
            if (dist[1][i][j] == -1) printf("   ");
            else printf("%3d",dist[1][i][j]);
        }
        printf("\n");
    }
}

int MapX (ChessPosition pst) {
    return pst.first + 8;
}

int MapY (ChessPosition pst) {
    return pst.second + 8;
}

int MapValue (ChessPosition pst, int (&map)[17][17]) {
    return map[pst.first + 8][pst.second + 8];
}

int DistValue (ChessPosition pst, int target) {
    return dist[target][pst.first + 8][pst.second + 8];
}

ChessPosition ToChessPosition (int x, int y) {
    return ChessPosition(x - 8, y - 8);
}

ChessPosition MoveTo (ChessPosition original, int moveId) {
    return ChessPosition(original.first + move[moveId][0], original.second + move[moveId][1]);
}

int MoveValue (ChessPosition begin, ChessPosition end, int target) {
    return DistValue(begin,target) - DistValue(end,target);
}

struct moveStruct { //记录一步移动的struct
    int beginNo; //moved chess' number in pst[]
    ChessPosition begin; //begin position
    ChessPosition end; //end position
    int target; //target area
    int value_dist; //move value (only dist)
    bool operator< (const moveStruct right) const {
        //if (target != right.target) //error
        //    while(1);
        //if (value != right.value)
            return value_dist < right.value_dist;
        //return DistValue(begin,target) < DistValue(right.begin, right.target); //落后的追上
    };
};

void executeMove (moveStruct &move, QVector<AlgoPlayer> &vec, int (&map)[17][17], int NowPlayer) {
    vec[NowPlayer].pst[move.beginNo] = move.end;
    map[MapX(move.begin)][MapY(move.begin)] = EMPTY;
    map[MapX(move.end)][MapY(move.end)] = NowPlayer;
    return;
}

void reverseMove (moveStruct &move, QVector<AlgoPlayer> &vec, int (&map)[17][17], int NowPlayer) {
    vec[NowPlayer].pst[move.beginNo] = move.begin;
    map[MapX(move.begin)][MapY(move.begin)] = NowPlayer;
    map[MapX(move.end)][MapY(move.end)] = EMPTY;
    return;
}

bool hasWon (QVector<AlgoPlayer> &vec, int NowPlayer) {
    for (int i = 0;i < 10;i++)
        if (DistValue(vec[NowPlayer].pst[i],vec[NowPlayer].target) > 4)
            return false;
    return true;
}

int calculateBack(AlgoPlayer &now) {
    int maxDist = -INF, tmpDist;
    for (int i = 0;i < 10;i++) {
        tmpDist = DistValue(now.pst[i],now.target);
        if (tmpDist > maxDist)
            maxDist = tmpDist;
    }
    return maxDist;
}

static bool seedFed = false;

pcc calculateGreedy(QVector<AlgoPlayer> vec){
    //printvec(vec);
    int map[17][17];
    bool vis[17][17][10];

    if (!seedFed) {
        srand(time(0));
        seedFed = true;
    }

    int vecSize = vec.size();
    /*//归一化，使vec[0].spawn = 4, vec[0].target = 1
    int alpha = vec[0].spawn - 4; //alpha * pi/3 :所有棋子逆时针旋转角度
    if (alpha < 0)
        alpha += 6;
    for (int i = 0;i < vecSize;i++) {
        vec[i].spawn = vec[i].spawn - alpha;
        if (vec[i].spawn <= 0)
            vec[i].spawn += 6;
        vec[i].target = vec[i].target - alpha;
        if (vec[i].target <= 0)
            vec[i].target += 6;
        for (int j = 0;j < 10;j++) {
            vec[i].pst[j] = rotateCounterclockwise(vec[i].pst[j], alpha);
        }
    }*/
    //printvec(vec);

    //map初始化
    for (int i = 0; i < 17;i++)
        for (int j = 0;j < 17;j++)
            map[i][j] = isWithinBoundary(ToChessPosition(i,j)) ? EMPTY : OUTOFBOUNDARY;
    for (int i = 0;i < vecSize;i++) {
        for (int j = 0;j < 10;j++) {
            map[MapX(vec[i].pst[j])][MapY(vec[i].pst[j])] = vec[i].spawn;
        }
    }
    //printmap();
    //printdist();

    memset(vis,0,sizeof(vis));
    std::queue<moveStruct> q;
    std::priority_queue<moveStruct> moveCandidate;

    //搜索行棋路线，现在玩家为vec[0]
    for (int i = 0;i < 10;i++) { //vec[0].pst[i]
        for (int j = 0;j < 6;j++) { //6个方向移动
            ChessPosition to = MoveTo(vec[0].pst[i],j);
            if (!isWithinBoundary(to))
                continue;
            if (MapValue(to, map) == EMPTY) { //move simply
                moveStruct tmp = {i, vec[0].pst[i], to, vec[0].target, MoveValue(vec[0].pst[i],to,vec[0].target)};
                moveCandidate.push(tmp);
            }
            else if (MapValue(to,map) > 0) { //occupied
                ChessPosition jumpto = jumpOver(vec[0].pst[i],to);
                if (!isWithinBoundary(jumpto))
                    continue;
                if (MapValue(jumpto,map) == EMPTY) { //jump
                    moveStruct tmp = {i, vec[0].pst[i], jumpto, vec[0].target, MoveValue(vec[0].pst[i],jumpto,vec[0].target)};
                    vis[MapX(jumpto)][MapY(jumpto)][i] = true;
                    q.push(tmp);
                }
            }
        }
    }
    while (!q.empty()) {
        moveStruct now = q.front();
        q.pop();
        moveCandidate.push(now);
        for (int j = 0;j < 6;j++) {
            ChessPosition to = MoveTo(now.end,j), jumpto = jumpOver(now.end,to);
            if (!isWithinBoundary(to))
                continue;
            if (!isWithinBoundary(jumpto))
                continue;
            if (!vis[MapX(jumpto)][MapY(jumpto)][now.beginNo] && MapValue(to,map) > 0 && MapValue(jumpto,map) == EMPTY) {
                moveStruct tmp = {now.beginNo, now.begin, jumpto, vec[0].target, MoveValue(vec[0].pst[now.beginNo],jumpto,vec[0].target)};
                vis[MapX(jumpto)][MapY(jumpto)][now.beginNo] = true;
                q.push(tmp);
            }
        }
    }
    moveStruct best = moveCandidate.top();
    std::cout << vec[0].name.toStdString();
    std::cout << " choices: " << moveCandidate.size();

    QVector<moveStruct> bests;
    while (!moveCandidate.empty()) {
        moveStruct tmp = moveCandidate.top();
        moveCandidate.pop();
        if (tmp.value_dist < best.value_dist || DistValue(tmp.begin,tmp.target) < DistValue(best.begin,best.target)) break;
        bests.push_back(tmp);
    }
    int best_id = rand() % bests.size();
    best = bests[best_id];
    std::cout << " bests.size: " << bests.size();
    std::cout << ",best_id: " << best_id;

    /*int cnt1 = 0,cnt2 = 0, cnt3 = 0;
    while (!moveCandidate.empty()) {
        moveStruct tmp = moveCandidate.top();
        moveCandidate.pop();
        if (tmp.value > 0) cnt1++;
        else if (tmp.value == 0) cnt2++;
        else cnt3++;
    }
    std::cout << " >0:" << cnt1 << " =0:" << cnt2 << " <0:" << cnt3;*/

    std::cout << " best.value: " << best.value_dist;

    std::cout << std::endl;
    //rotate back
    //return pcc(rotateCounterclockwise(vec[0].pst[best.beginNo],6 - alpha), rotateCounterclockwise(best.end, 6 - alpha));
    return pcc(vec[0].pst[best.beginNo],best.end);
}

static int dfsNode = 0, totalCandidate = 0;
static int potentialCache = 0;

int minimaxDfs(QVector<AlgoPlayer> &vec, int (&map)[17][17], bool (&vis)[17][17][10], int depth, double nodeValue, int oldBack, int oldPotential, int situation, double alpha, double beta, int MinimaxDepth) {
    dfsNode++;
    if (dfsNode % 1000000 == 0)
        std::cout << "dfsNode = " << dfsNode << ", MinimaxDepth = " << MinimaxDepth << ", depth = " << depth << std::endl;

    int vecSize = vec.size();
    int NowPlayer = depth % vecSize;
    int nodeType = NowPlayer == 0 ? MAXNODE : MINNODE;
    bool writePotentialCache = (nodeType == MAXNODE) && (depth < MinimaxDepth) && (MinimaxDepth - depth < vecSize);

    memset(vis,0,sizeof(vis));
    std::queue<moveStruct> q;
    std::queue<moveStruct> moveCandidate;
    int potentialValue[10] = {0,0,0,0,0,0,0,0,0,0},totalPotential = 0;

    //搜索行棋路线，现在玩家为vec[NowPlayer]
    for (int i = 0;i < 10;i++) { //vec[NowPlayer].pst[i]
        for (int j = 0;j < 6;j++) { //6个方向移动
            ChessPosition to = MoveTo(vec[NowPlayer].pst[i],j);
            if (!isWithinBoundary(to))
                continue;
            if (MapValue(to, map) == EMPTY) { //move simply
                moveStruct tmp = {i, vec[NowPlayer].pst[i], to, vec[NowPlayer].target, MoveValue(vec[NowPlayer].pst[i],to,vec[NowPlayer].target)};
                moveCandidate.push(tmp);
                if ((depth == MinimaxDepth && nodeType == MAXNODE) || writePotentialCache) {
                    if (tmp.value_dist > potentialValue[tmp.beginNo])
                        potentialValue[tmp.beginNo] = tmp.value_dist;
                }
            }
            else if (MapValue(to,map) > 0) { //occupied
                ChessPosition jumpto = jumpOver(vec[NowPlayer].pst[i],to);
                if (!isWithinBoundary(jumpto))
                    continue;
                if (MapValue(jumpto,map) == EMPTY) { //jump
                    moveStruct tmp = {i, vec[NowPlayer].pst[i], jumpto, vec[NowPlayer].target, MoveValue(vec[NowPlayer].pst[i],jumpto,vec[NowPlayer].target)};
                    vis[MapX(jumpto)][MapY(jumpto)][i] = true;
                    q.push(tmp);
                }
            }
        }
    }
    while (!q.empty()) {
        moveStruct now = q.front();
        q.pop();
        if (now.value_dist >= 0 || (!IGNORERETREAT))
            moveCandidate.push(now);
        if ((depth == MinimaxDepth && nodeType == MAXNODE) || writePotentialCache) {
            if (now.value_dist > potentialValue[now.beginNo])
                potentialValue[now.beginNo] = now.value_dist;
        }
        for (int j = 0;j < 6;j++) {
            ChessPosition to = MoveTo(now.end,j), jumpto = jumpOver(now.end,to);
            if (!isWithinBoundary(to))
                continue;
            if (!isWithinBoundary(jumpto))
                continue;
            if (!vis[MapX(jumpto)][MapY(jumpto)][now.beginNo] && MapValue(to,map) > 0 && MapValue(jumpto,map) == EMPTY) {
                moveStruct tmp = {now.beginNo, now.begin, jumpto, vec[NowPlayer].target, MoveValue(vec[NowPlayer].pst[now.beginNo],jumpto,vec[NowPlayer].target)};
                vis[MapX(jumpto)][MapY(jumpto)][now.beginNo] = true;
                q.push(tmp);
            }
        }
    }

    int tmpValue;
    if (nodeType == MAXNODE) {
        while (!moveCandidate.empty()) {
            moveStruct now = moveCandidate.front();
            moveCandidate.pop();
            if (now.value_dist < 0 && IGNORERETREAT) // 不考虑后退
                continue;
            totalCandidate++;

            executeMove(now,vec,map,NowPlayer);
            if (hasWon(vec,NowPlayer))
                tmpValue = INF;
            else if (depth == MinimaxDepth) {
                std::sort(potentialValue,potentialValue + 10);
                int j = 1;
                for (int i = 0;i < POTENTIALCOUNT;i++) {
                    totalPotential += j * potentialValue[i];
                    j *= POTENTIALDECAY;
                }
                tmpValue = nodeValue + now.value_dist * (vecSize - 1) + (oldBack - calculateBack(vec[0])) * RATEOFBACK[situation] + totalPotential * RATEOFPOTENTIAL[situation];
            }
            else {
                if (writePotentialCache) {
                    std::sort(potentialValue,potentialValue + 10);
                    int j = 1;
                    for (int i = 0;i < POTENTIALCOUNT;i++) {
                        totalPotential += j * potentialValue[i];
                        j *= POTENTIALDECAY;
                    }
                    potentialCache = totalPotential;
                }
                tmpValue = minimaxDfs(vec,map,vis,depth + 1,nodeValue + now.value_dist * (vecSize - 1),oldBack,oldPotential,situation,alpha,beta,MinimaxDepth);
            }
            reverseMove(now,vec,map,NowPlayer);

            if (tmpValue == PRUNED)
                continue;
            if (tmpValue > alpha) {
                alpha = tmpValue;
                if (alpha >= beta)
                    return PRUNED;
            }
        }
        return alpha;
    }
    else { //nodeType == MINNODE
        while (!moveCandidate.empty()) {
            moveStruct now = moveCandidate.front();
            moveCandidate.pop();
            if (now.value_dist < 0 && IGNORERETREAT)
                continue;
            totalCandidate++;

            executeMove(now,vec,map,NowPlayer);
            if (hasWon(vec,NowPlayer))
                tmpValue = -INF;
            else if (depth == MinimaxDepth) {
                /*{memset(vis,0,sizeof(vis)); //calculatePotential(), TOO SLOW
                std::queue<moveStruct> q2;
                std::queue<moveStruct> moveCandidate2;

                //搜索行棋路线，现在玩家为vec[0]
                for (int i = 0;i < 10;i++) { //vec[0].pst[i]
                    for (int j = 0;j < 6;j++) { //6个方向移动
                        ChessPosition to = MoveTo(vec[0].pst[i],j);
                        if (!isWithinBoundary(to))
                            continue;
                        if (MapValue(to, map) == EMPTY) { //move simply
                            moveStruct tmp = {i, vec[0].pst[i], to, vec[0].target, MoveValue(vec[0].pst[i],to,vec[0].target)};
                            moveCandidate2.push(tmp);
                            if (tmp.value_dist > potentialValue[tmp.beginNo])
                                potentialValue[tmp.beginNo] = tmp.value_dist;
                        }
                        else if (MapValue(to,map) > 0) { //occupied
                            ChessPosition jumpto = jumpOver(vec[0].pst[i],to);
                            if (!isWithinBoundary(jumpto))
                                continue;
                            if (MapValue(jumpto,map) == EMPTY) { //jump
                                moveStruct tmp = {i, vec[0].pst[i], jumpto, vec[0].target, MoveValue(vec[0].pst[i],jumpto,vec[0].target)};
                                vis[MapX(jumpto)][MapY(jumpto)][i] = true;
                                q2.push(tmp);
                            }
                        }
                    }
                }
                while (!q2.empty()) {
                    moveStruct now = q2.front();
                    q2.pop();
                    moveCandidate2.push(now);
                    if (now.value_dist > potentialValue[now.beginNo])
                        potentialValue[now.beginNo] = now.value_dist;
                    for (int j = 0;j < 6;j++) {
                        ChessPosition to = MoveTo(now.end,j), jumpto = jumpOver(now.end,to);
                        if (!isWithinBoundary(to))
                            continue;
                        if (!isWithinBoundary(jumpto))
                            continue;
                        if (!vis[MapX(jumpto)][MapY(jumpto)][now.beginNo] && MapValue(to,map) > 0 && MapValue(jumpto,map) == EMPTY) {
                            moveStruct tmp = {now.beginNo, now.begin, jumpto, vec[0].target, MoveValue(vec[0].pst[now.beginNo],jumpto,vec[0].target)};
                            vis[MapX(jumpto)][MapY(jumpto)][now.beginNo] = true;
                            q2.push(tmp);
                        }
                    }
                }
                std::sort(potentialValue,potentialValue + 10);
                int j = 1;
                for (int i = 0;i < POTENTIALCOUNT;i++) {
                    totalPotential += j * potentialValue[i];
                    j *= POTENTIALDECAY;
                }}*/
                totalPotential = potentialCache;
                tmpValue = nodeValue - now.value_dist + (oldBack - calculateBack(vec[0])) * RATEOFBACK[situation] + totalPotential * RATEOFPOTENTIAL[situation];
            }
            else
                tmpValue = minimaxDfs(vec,map,vis,depth + 1,nodeValue - now.value_dist,oldBack,oldPotential,situation,alpha,beta,MinimaxDepth);
            reverseMove(now,vec,map,NowPlayer);

            if (tmpValue == PRUNED)
                continue;
            if (tmpValue < beta) {
                beta = tmpValue;
                if (alpha >= beta)
                    return PRUNED;
            }
        }
        return beta;
    }
}

pcc calculateMinimax(QVector<AlgoPlayer> vec, int referenceMinimaxDepth){ // ALPHA-BETA'S PERFORANCE SIGNIFICANTLY DECREASES WHEN > 2 PLAYERS
    dfsNode = 0;
    totalCandidate = 0;
    potentialCache = 0;
    std::cout << "referenceMinimaxDepth = " << referenceMinimaxDepth << std::endl;
    clock_t begin,last,present,end;
    begin = clock();
    bool isBegin = true;
    for(int i = 0;i < 10;i++)
        if (DistValue(vec[0].pst[i],vec[0].target) > 4) {
            isBegin = false;
            break;
        }
    if (isBegin && Steps > 6) Steps = 0;
    Steps++;

    int MinimaxDepth = referenceMinimaxDepth;
    int map[17][17];
    bool vis[17][17][10];
    int vecSize = vec.size();
    for (int i = 0; i < 17;i++)
        for (int j = 0;j < 17;j++)
            map[i][j] = isWithinBoundary(ToChessPosition(i,j)) ? EMPTY : OUTOFBOUNDARY;
    for (int i = 0;i < vecSize;i++) {
        for (int j = 0;j < 10;j++) {
            map[MapX(vec[i].pst[j])][MapY(vec[i].pst[j])] = vec[i].spawn;
        }
    }
    int Situation = Steps < OPENINGSTEPS[vecSize] ? OPENING : (Steps > ENDGAMESTEPS[vecSize] ? ENDGAME : MIDDLE);

    memset(vis,0,sizeof(vis));
    std::queue<moveStruct> q;
    std::queue<moveStruct> moveCandidate;
    int potentialValue[10] = {0,0,0,0,0,0,0,0,0,0},totalPotential = 0;

    //搜索行棋路线，现在玩家为vec[0]
    for (int i = 0;i < 10;i++) { //vec[0].pst[i]
        for (int j = 0;j < 6;j++) { //6个方向移动
            ChessPosition to = MoveTo(vec[0].pst[i],j);
            if (!isWithinBoundary(to))
                continue;
            if (MapValue(to, map) == EMPTY) { //move simply
                moveStruct tmp = {i, vec[0].pst[i], to, vec[0].target, MoveValue(vec[0].pst[i],to,vec[0].target)};
                if (tmp.value_dist >= 0 || !IGNORERETREAT)
                    moveCandidate.push(tmp);
                if (tmp.value_dist > potentialValue[tmp.beginNo])
                    potentialValue[tmp.beginNo] = tmp.value_dist;
            }
            else if (MapValue(to,map) > 0) { //occupied
                ChessPosition jumpto = jumpOver(vec[0].pst[i],to);
                if (!isWithinBoundary(jumpto))
                    continue;
                if (MapValue(jumpto,map) == EMPTY) { //jump
                    moveStruct tmp = {i, vec[0].pst[i], jumpto, vec[0].target, MoveValue(vec[0].pst[i],jumpto,vec[0].target)};
                    vis[MapX(jumpto)][MapY(jumpto)][i] = true;
                    q.push(tmp);
                }
            }
        }
    }
    while (!q.empty()) {
        moveStruct now = q.front();
        q.pop();
        //std::cout << now.begin.first << "," << now.begin.second << " -> " << now.end.first << "," << now.end.second << std::endl;
        if (now.value_dist >= 0 || !IGNORERETREAT)
            moveCandidate.push(now);
        if (now.value_dist > potentialValue[now.beginNo])
            potentialValue[now.beginNo] = now.value_dist;
        for (int j = 0;j < 6;j++) {
            ChessPosition to = MoveTo(now.end,j), jumpto = jumpOver(now.end,to);
            if (!isWithinBoundary(to))
                continue;
            if (!isWithinBoundary(jumpto))
                continue;
            if (!vis[MapX(jumpto)][MapY(jumpto)][now.beginNo] && MapValue(to,map) > 0 && MapValue(jumpto,map) == EMPTY) {
                moveStruct tmp = {now.beginNo, now.begin, jumpto, vec[0].target, MoveValue(vec[0].pst[now.beginNo],jumpto,vec[0].target)};
                vis[MapX(jumpto)][MapY(jumpto)][now.beginNo] = true;
                q.push(tmp);
            }
        }
    }
    std::sort(potentialValue,potentialValue + 10);
    int j = 1;
    for (int i = 0;i < POTENTIALCOUNT;i++) {
        totalPotential += j * potentialValue[i];
        j *= POTENTIALDECAY;
    }

    int maxValue = -INF; //root is MAXNODE
    int unsearchedCandidateCount = moveCandidate.size();
    int estimateTime,restTime;
    double depthChange;
    moveStruct maxMove;
    last = clock();
    std::cout << "root's CandidateSize = " << moveCandidate.size() << std::endl;
    while (!moveCandidate.empty()) {
        std::cout << "unsearchedCandidateCount = " << unsearchedCandidateCount << std::endl;
        moveStruct now = moveCandidate.front();
        moveCandidate.pop();
        executeMove(now,vec,map,0);
        int minimaxValue = minimaxDfs(vec,map,vis,1,now.value_dist,calculateBack(vec[0]),totalPotential,Situation,-INF,INF,MinimaxDepth);
        //std::cout << "after dfs" << std::endl;
        if (minimaxValue > maxValue) {
            maxValue = minimaxValue;
            maxMove = now;
        }
        reverseMove(now,vec,map,0);
        unsearchedCandidateCount--;
        present = clock();
        if (present - begin >= TIMELIMIT) {
            MinimaxDepth--;
            break;
        }
        estimateTime = (present - last + 1) * unsearchedCandidateCount;
        restTime = TIMELIMIT - (present - begin);
        std::cout << "thisTime = " << (present - last + 1) << ", restTime = " << restTime << ", estimateTime = " << estimateTime << ", totalCandidate = " << totalCandidate << ", dfsNode = " << dfsNode << std::endl;
        depthChange = floor(log2(restTime * 1.0 / estimateTime) / log2(totalCandidate * 1.0 / dfsNode));
        std::cout << "depthChange = " << depthChange;
        if (depthChange > 2)
            depthChange = 2;
        if (depthChange < -2)
            depthChange = -2;
        if (unsearchedCandidateCount >= 2 || depthChange < 0) {
            MinimaxDepth += (int)depthChange;
        }
        std::cout << ", newMinimaxDepth = " << MinimaxDepth << std::endl;
        last = present;
    }
    if (referenceMinimaxDepth == MinimaxDepthLimit)
        MinimaxDepthLimit = MinimaxDepth;

    std::cout << "total dfsNode = " << dfsNode;
    end = clock();
    std::cout << ", time = " << end - begin << "ms" << std::endl << std::endl;

    //std::cout << maxMove.begin.first << "," << maxMove.begin.second << " -> ";
    //std::cout << maxMove.end.first << "," << maxMove.end.second << std::endl;
    return pcc(maxMove.begin,maxMove.end);
}

pcc calculateAuto(QVector<AlgoPlayer> vec) {
    return calculateMinimax(vec,MinimaxDepthLimit);
}

pcc calculatePre(QVector<AlgoPlayer> vec) {
    return calculateMinimax(vec,MINIMAXDEPTHFORPRE);
}

Agent_algorithm *get_agent_algorithm(QString func)
{
    if (func == "auto") {
        return &calculateAuto;
    }
    else if (func == "pre") {
        return &calculatePre;
    }
    else if (func == "greedy") {
        return &calculateGreedy;
    }
    return nullptr;
}
