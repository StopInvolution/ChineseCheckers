#ifndef SETTINGS_H
#define SETTINGS_H

#include <utility>

typedef std::pair<int,int> ChessPostion;


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
    const int r=14;
}//namespace Marble


namespace board{
    const double spacing = 37;
    const double iHatX = 1*spacing;
    const double iHatY = 0*spacing;
    const double jHatX = -0.5*spacing;
    const double jHatY = -0.83*spacing;
    const int OriginX = 471;
    const int OriginY = 312;
    const int boundary = 4;
    const int indexBoundary = 8;

// 改用整数了，这个没什么实际意义干脆删了
//    const ChessPostion iHat(iHatX,iHatY);
//    const ChessPostion jHat(jHatX,jHatY);

    const ChessPostion OriginPoint(OriginX,OriginY);

// 6个出生区域内每个点的坐标,格式为(x1,y1,x2,y2,...,x10,y10)
    const int spawnPst[7][20]={{},{-4,-8,-4,-7,-3,-7,-4,-6,-3,-6,-2,-6,-4,-5,-3,-5,-2,-5,-1,-5},
                         {1,-4,2,-4,3,-4,4,-4,2,-3,3,-3,4,-3,3,-2,4,-2,4,-1},
                         {5,1,5,2,6,2,5,3,6,3,7,3,5,4,6,4,7,4,8,4},
                         {1,5,2,5,3,5,4,5,2,6,3,6,4,6,3,7,4,7,4,8},
                         {-4,1,-4,2,-3,2,-4,3,-3,3,-2,3,-4,4,-3,4,-2,4,-1,4},
                         {-8,-4,-7,-4,-6,-4,-5,-4,-7,-3,-6,-3,-5,-3,-6,-2,-5,-2,-5,-1}};

// 不同对局人数下的起点-终点区域编号
    const int playerSpawn[7][6]={{},{1},{1,4},{1,3,5},{1,2,4,5},{},{1,2,3,4,5,6}};
    const int playerTarget[7][6]={{},{4},{4,1},{3,5,1},{4,5,1,2},{},{2,3,4,5,6,1}};
    const int dx[]={1,1,0,-1,-1,0};
    const int dy[]={0,1,1,0,-1,-1};

// 所有121个合法点的坐标
    const int pst[242]={-4,-8,-4,-7,-3,-7,-4,-6,-3,-6,-2,-6,-4,-5,-3,-5,-2,-5,-1,-5,-8,-4,-7,-4,-6,-4,-5,-4,-4,-4,-3,-4,-2,-4,-1,-4,0,-4,1,-4,2,-4,3,-4,4,-4,-7,-3,-6,-3,-5,-3,-4,-3,-3,-3,-2,-3,-1,-3,0,-3,1,-3,2,-3,3,-3,4,-3,-6,-2,-5,-2,-4,-2,-3,-2,-2,-2,-1,-2,0,-2,1,-2,2,-2,3,-2,4,-2,-5,-1,-4,-1,-3,-1,-2,-1,-1,-1,0,-1,1,-1,2,-1,3,-1,4,-1,-4,0,-3,0,-2,0,-1,0,0,0,1,0,2,0,3,0,4,0,-4,1,-3,1,-2,1,-1,1,0,1,1,1,2,1,3,1,4,1,5,1,-4,2,-3,2,-2,2,-1,2,0,2,1,2,2,2,3,2,4,2,5,2,6,2,-4,3,-3,3,-2,3,-1,3,0,3,1,3,2,3,3,3,4,3,5,3,6,3,7,3,-4,4,-3,4,-2,4,-1,4,0,4,1,4,2,4,3,4,4,4,5,4,6,4,7,4,8,4,1,5,2,5,3,5,4,5,2,6,3,6,4,6,3,7,4,7,4,8};
}//namespace Board


#endif // SETTINGS_H
