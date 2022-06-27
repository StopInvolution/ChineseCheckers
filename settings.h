#ifndef SETTINGS_H
#define SETTINGS_H

#include <utility>
#include <QString>
#include <QVector>
#include "mainwindow.h"

typedef std::pair<int,int> ChessPosition;
typedef std::pair<QString,QString> pss;
typedef std::pair<ChessPosition,ChessPosition> pcc;

namespace CB{
    const int LOCAL=0;
    const int SERVER=1;
    const int CLIENT=2;
}

namespace color{
const int hint = 0;
const int red = 1;
const int blue = 2;
const int yellow = 3;
const int orange = 4;
const int pink = 5;
const int purple = 6;

}//namesapce Color


namespace marbleinfo{
/**
 * @brief r 棋子显示时的半径
 */
const int r=14;
}//namespace Marble


namespace board{

/**
 * @brief spacing 实际上两个棋子显示时的间距
 */
const double spacing = 37;
const double iHatX = 1*spacing;
const double iHatY = 0*spacing;
const double jHatX = 0.5*spacing;
const double jHatY = 0.83*spacing;
const int OriginX = 471;
const int OriginY = 314;
const int boundary = 4;
const int indexBoundary = 8;

// 改用整数了，这个没什么实际意义干脆删了
//    const ChessPosition iHat(iHatX,iHatY);
//    const ChessPosition jHat(jHatX,jHatY);

const ChessPosition OriginPoint(OriginX,OriginY);

/**
 * @brief spawnPst 6个出生区域内每个点的坐标,格式为(x1,y1,x2,y2,...,x10,y10)
 */
const int spawnPst[7][20]={{},
                           {-4,5,-3,5,-2,5,-1,5,-4,6,-3,6,-2,6,-4,7,-3,7,-4,8},
                           {4,1,3,2,4,2,2,3,3,3,4,3,1,4,2,4,3,4,4,4},
                           {5,-4,6,-4,7,-4,8,-4,5,-3,6,-3,7,-3,5,-2,6,-2,5,-1},
                           {4,-8,3,-7,4,-7,2,-6,3,-6,4,-6,1,-5,2,-5,3,-5,4,-5},
                           {-4,-4,-3,-4,-2,-4,-1,-4,-4,-3,-3,-3,-2,-3,-4,-2,-3,-2,-4,-1},
                           {-5,1,-6,2,-5,2,-7,3,-6,3,-5,3,-8,4,-7,4,-6,4,-5,4}};

// 不同对局人数下的起点-终点区域编号
/**
 * @brief playerSpawn 不同总数下，玩家应该处于的起点
 */
const int playerSpawn[7][6]={{},{1},{1,4},{1,3,5},{1,2,4,5},{},{1,2,3,4,5,6}};

/**
 * @brief playerSpawn 不同总数下，玩家应该处于的终点
 */
const int playerTarget[7][6]={{},{4},{4,1},{3,5,1},{4,5,1,2},{},{2,3,4,5,6,1}};
const int dx[]={1,0,-1,-1,0,1};
const int dy[]={0,1,1,0,-1,-1};
const int pst[]={4,-8,3,-7,4,-7,2,-6,3,-6,4,-6,1,-5,2,-5,3,-5,4,-5,-4,-4,-3,-4,-2,-4,-1,-4,0,-4,1,-4,2,-4,3,-4,4,-4,5,-4,6,-4,7,-4,8,-4,-4,-3,-3,-3,-2,-3,-1,-3,0,-3,1,-3,2,-3,3,-3,4,-3,5,-3,6,-3,7,-3,-4,-2,-3,-2,-2,-2,-1,-2,0,-2,1,-2,2,-2,3,-2,4,-2,5,-2,6,-2,-4,-1,-3,-1,-2,-1,-1,-1,0,-1,1,-1,2,-1,3,-1,4,-1,5,-1,-4,0,-3,0,-2,0,-1,0,0,0,1,0,2,0,3,0,4,0,-5,1,-4,1,-3,1,-2,1,-1,1,0,1,1,1,2,1,3,1,4,1,-6,2,-5,2,-4,2,-3,2,-2,2,-1,2,0,2,1,2,2,2,3,2,4,2,-7,3,-6,3,-5,3,-4,3,-3,3,-2,3,-1,3,0,3,1,3,2,3,3,3,4,3,-8,4,-7,4,-6,4,-5,4,-4,4,-3,4,-2,4,-1,4,0,4,1,4,2,4,3,4,4,4,-4,5,-3,5,-2,5,-1,5,-4,6,-3,6,-2,6,-4,7,-3,7,-4,8};

}//namespace Board

/**
 * @note change with ini file
 */
namespace Network {
extern int port;
extern QString ip[];
extern double resTime;
extern int serverPort;
}

/**
 * @brief The AlgoPlayer class 存储起点，终点和10个棋子的位置
 */
class AlgoPlayer{
public:
    AlgoPlayer()=default;
    /**
     * @brief AlgoPlayer 生成一个初始状态的数据
     * @param s 位置序号
     */
    AlgoPlayer(int s);

    QString name;
    int spawn,target;
    ChessPosition pst[10];
};

using Agent_algorithm = pcc(QVector<AlgoPlayer>);
#endif // SETTINGS_H
