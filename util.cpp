#include "util.h"
#include <iostream>

ChessPostion boardTransform(ChessPostion var){
    double x=var.first,y=var.second;
    return ChessPostion(x*board::iHatX+y*board::jHatX+board::OriginX,x*board::iHatY+y*board::jHatY+board::OriginY);
}

ChessPostion boardTransform(int x,int y){
    return ChessPostion(x*board::iHatX+y*board::jHatX+board::OriginX,x*board::iHatY+y*board::jHatY+board::OriginY);
}

QString getColorName(int color){
    QString colorName;
    switch(color){
    case 0:
        colorName="hint";
        break;
    case 1:
        colorName="red";
        break;
    case 2:
        colorName="blue";
        break;
    case 3:
        colorName="yellow";
        break;
    case 4:
        colorName="orange";
        break;
    case 5:
        colorName="pink";
        break;
    case 6:
        colorName="purple";
        break;
    }
    return colorName;
}

bool isCollinear(const ChessPostion &arg1, const ChessPostion &arg2)
{
    int x(arg1.first),y(arg1.second),xx(arg2.first),yy(arg2.second);
    return x==xx || y==yy || y-x==yy-xx;
}

bool isCollinear(const ChessPostion &arg1, const ChessPostion &arg2, const ChessPostion &arg3)
{
    int x=arg2.first-arg1.first,y=arg2.second-arg1.second,xx=arg3.first-arg2.first,yy=arg3.second-arg2.second;
//    std::cerr<<x*yy-y*xx<<std::endl;
    return isCollinear(arg1,arg2) & isCollinear(arg2,arg3) & (x*yy==xx*y) & ((x * xx > 0) | (y * yy > 0));
}

bool isWithinBoundary(const ChessPostion &arg)
{
    return (arg.first<=board::boundary && arg.second>=-board::boundary && arg.second<=arg.first+board::boundary) ||
            (arg.first>=-board::boundary && arg.second<=board::boundary && arg.second>=arg.first-board::boundary);
}

ChessPostion jumpOver(const ChessPostion &arg1, const ChessPostion &arg2){
    return ChessPostion(2*arg2.first-arg1.first,2*arg2.second-arg1.second);
}

void outChessPostion(ChessPostion p){
    std::cout<<p.first<<" "<<p.second<<std::endl;
}

QString getQColor(int color)
{
    switch(color){
    case 0:
        // hint
        return QString("color:#202020;");
        break;
    case 1:
        // red
        return QString("color:#e41524;");
        break;
    case 2:
        // blue
        return QString("color:#0f0eca;");
        break;
    case 3:
        // yellow
        return QString("color:#15e815;");
        break;
    case 4:
        // orange
        return QString("color:#e8a915;");
        break;
    case 5:
        // pink
        return QString("color:#e11583;");
        break;
    case 6:
        return QString("color:#972398;");
        break;
    }
    return QString("color:#000000;");
}
