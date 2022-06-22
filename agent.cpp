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

static const int MINIMAXDEPTH = 6;
static const int INF = 2147483647;
static const int PRUNED = 998244353;
static const int MINNODE = -1, MAXNODE = 1;
static const bool IGNORERETREAT = true;

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
    int value; //move value
    bool operator< (const moveStruct right) const {
        if (target != right.target) //error
            while(1);
        if (value != right.value)
            return value < right.value;
        return DistValue(begin,target) < DistValue(right.begin, right.target); //落后的追上
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
        if (tmp.value < best.value || DistValue(tmp.begin,tmp.target) < DistValue(best.begin,best.target)) break;
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

    std::cout << " best.value: " << best.value;

    std::cout << std::endl;
    //rotate back
    //return pcc(rotateCounterclockwise(vec[0].pst[best.beginNo],6 - alpha), rotateCounterclockwise(best.end, 6 - alpha));
    return pcc(vec[0].pst[best.beginNo],best.end);
}

static int dfsNode = 0;

int minimaxDfs(QVector<AlgoPlayer> &vec, int (&map)[17][17], bool (&vis)[17][17][10] ,int depth, int nodeValue, int alpha, int beta) {

    dfsNode++;
    if (dfsNode % 1000000 == 0) std::cout << "dfsNode = " << dfsNode << std::endl;

    int NowPlayer = depth % vec.size();
    int nodeType = NowPlayer == 0 ? MAXNODE : MINNODE;

    memset(vis,0,sizeof(vis));
    std::queue<moveStruct> q;
    std::priority_queue<moveStruct> moveCandidate;

    //搜索行棋路线，现在玩家为vec[NowPlayer]
    for (int i = 0;i < 10;i++) { //vec[NowPlayer].pst[i]
        for (int j = 0;j < 6;j++) { //6个方向移动
            ChessPosition to = MoveTo(vec[NowPlayer].pst[i],j);
            if (!isWithinBoundary(to))
                continue;
            if (MapValue(to, map) == EMPTY) { //move simply
                moveStruct tmp = {i, vec[NowPlayer].pst[i], to, vec[NowPlayer].target, MoveValue(vec[NowPlayer].pst[i],to,vec[NowPlayer].target)};
                moveCandidate.push(tmp);
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
        if (now.value >= 0 || (!IGNORERETREAT))
            moveCandidate.push(now);
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
            moveStruct now = moveCandidate.top();
            moveCandidate.pop();
            if (now.value < 0 && IGNORERETREAT) // 不考虑后退
                break;

            executeMove(now,vec,map,NowPlayer);
            if (hasWon(vec,NowPlayer))
                tmpValue = INF;
            else if (depth == MINIMAXDEPTH)
                tmpValue = nodeValue + now.value;
            else
                tmpValue = minimaxDfs(vec,map,vis,depth + 1,nodeValue + now.value,alpha,INF);
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
            moveStruct now = moveCandidate.top();
            moveCandidate.pop();
            if (now.value < 0 && IGNORERETREAT)
                break;

            executeMove(now,vec,map,NowPlayer);
            if (hasWon(vec,NowPlayer))
                tmpValue = -INF;
            else if (depth == MINIMAXDEPTH)
                tmpValue = nodeValue - now.value;
            else
                tmpValue = minimaxDfs(vec,map,vis,depth + 1,nodeValue - now.value,-INF,beta);
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

pcc calculateMinimax(QVector<AlgoPlayer> vec){ // IMPORTANT: ONLY FOR 2 PLAYERS NOW
    dfsNode = 0;

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

    int maxValue = -INF; //root is MAXNODE
    moveStruct maxMove;
    while (!moveCandidate.empty()) {
        moveStruct now = moveCandidate.top();
        moveCandidate.pop();
        if (now.value < 0 && IGNORERETREAT) // 不考虑后退
            break;
        executeMove(now,vec,map,0);
        int minimaxValue = minimaxDfs(vec,map,vis,1,now.value,-INF,INF);
        if (minimaxValue > maxValue) {
            maxValue = minimaxValue;
            maxMove = now;
        }
        reverseMove(now,vec,map,0);
    }
    std::cout << "total dfsNode = " << dfsNode << std::endl;
    return pcc(vec[0].pst[maxMove.beginNo],maxMove.end);
}

Agent_algorithm *get_agent_algorithm(QString func)
{
    if(func=="auto"){
        return &calculateMinimax;
    }
    return nullptr;
}
