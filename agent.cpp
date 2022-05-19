#include "util.h"
#include "agent.h"
#include <QDebug>
#include <QVector>
#include <cmath>
#include <queue>
#include <cstring>
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
ChessPosition rotateCounterclockwise (ChessPosition begin, int alpha) {
    //qDebug() << "rotateCounterclockwise: begin = " << begin.first << " " << begin.second << ", alpha = " << alpha;
    if (alpha == 0)
        return begin;
    const double pi = acos(-1);
    double cosalpha = cos(alpha * pi / 3.0), sinalpha = sin(alpha * pi / 3.0);
    double x2 = begin.first + 0.5 * begin.second, y2 = sqrt(3) / 2.0 * begin.second;
    double s2 = x2 * cosalpha - y2 * sinalpha, t2 = x2 * sinalpha + y2 * cosalpha;
    double s = s2 - sqrt(3) / 3.0 * t2, t = 2.0 * sqrt(3) / 3.0 * t2;
    //qDebug() << "rotate finish, s = " << s << ", t = " << t;
    //while(1);
    return ChessPosition(round(s), round(t));
}

const int OUTOFBOUNDARY = -1;
const int EMPTY = 0;
const int move[6][2] = {{-1, 1},{ 0, 1},
                        {-1, 0},        { 1, 0},
                                { 0,-1},{ 1,-1}};

const int dist[17][17] = {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 9,-1,-1,-1,-1},
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
                          {-1,-1,-1,-1,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};

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

void printmap(int** &map) {
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
            if (dist[i][j] == -1) printf("   ");
            else printf("%3d",dist[i][j]);
        }
        printf("\n");
    }
}

int ChessPositionToMapX (ChessPosition pst) {
    return pst.first + 8;
}

int ChessPositionToMapY (ChessPosition pst) {
    return pst.second + 8;
}

int ChessPositionToMapValue (ChessPosition pst, int (&map)[17][17]) {
    return map[pst.first + 8][pst.second + 8];
}

int ChessPositionToDist (ChessPosition pst) {
    return dist[pst.first + 8][pst.second + 8];
}

ChessPosition MapXYToChessPosition (int x, int y) {
    return ChessPosition(x - 8, y - 8);
}

ChessPosition MoveTo (ChessPosition original, int moveId) {
    return ChessPosition(original.first + move[moveId][0], original.second + move[moveId][1]);
}

int MoveValue (ChessPosition begin, ChessPosition end) {
    return ChessPositionToDist(begin) - ChessPositionToDist(end);
}

struct moveStruct {
    int beginNo;
    ChessPosition begin;
    ChessPosition c;
    int value;
    bool operator< (const moveStruct right) const {
        if (value != right.value)
            return value < right.value;
        return ChessPositionToDist(begin) < ChessPositionToDist(right.begin);
    };
};

pcc calculate(QVector<AlgoPlayer> vec){
    //printvec(vec);
    int map[17][17];
    bool vis[17][17][10];

    int vecSize = vec.size();
    //归一化，使vec[0].spawn = 4, vec[0].target = 1
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
    }
    //printvec(vec);

    //map初始化
    for (int i = 0; i < 17;i++)
        for (int j = 0;j < 17;j++)
            map[i][j] = isWithinBoundary(MapXYToChessPosition(i,j)) ? EMPTY : OUTOFBOUNDARY;
    for (int i = 0;i < vecSize;i++) {
        for (int j = 0;j < 10;j++) {
            map[ChessPositionToMapX(vec[i].pst[j])][ChessPositionToMapY(vec[i].pst[j])] = vec[i].spawn;
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
            if (ChessPositionToMapValue(to, map) == EMPTY) { //move simply
                moveStruct tmp = {i, vec[0].pst[i], to, MoveValue(vec[0].pst[i],to)};
                moveCandidate.push(tmp);
            }
            else if (ChessPositionToMapValue(to,map) > 0) { //occupied
                ChessPosition jumpto = jumpOver(vec[0].pst[i],to);
                if (!isWithinBoundary(jumpto))
                    continue;
                if (ChessPositionToMapValue(jumpto,map) == EMPTY) { //jump
                    moveStruct tmp = {i, vec[0].pst[i], jumpto, MoveValue(vec[0].pst[i],jumpto)};
                    vis[ChessPositionToMapX(jumpto)][ChessPositionToMapY(jumpto)][i] = true;
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
            ChessPosition to = MoveTo(now.c,j), jumpto = jumpOver(now.c,to);
            if (!isWithinBoundary(to))
                continue;
            if (!isWithinBoundary(jumpto))
                continue;
            if (!vis[ChessPositionToMapX(jumpto)][ChessPositionToMapY(jumpto)][now.beginNo] &&
                ChessPositionToMapValue(to,map) > 0 && ChessPositionToMapValue(jumpto,map) == EMPTY) {
                moveStruct tmp = {now.beginNo, now.begin, jumpto, MoveValue(vec[0].pst[now.beginNo], jumpto)};
                vis[ChessPositionToMapX(jumpto)][ChessPositionToMapY(jumpto)][now.beginNo] = true;
                q.push(tmp);
            }
        }
    }
    moveStruct best = moveCandidate.top();

    //rotate back
    return pcc(rotateCounterclockwise(vec[0].pst[best.beginNo],6 - alpha), rotateCounterclockwise(best.c, 6 - alpha));
}
