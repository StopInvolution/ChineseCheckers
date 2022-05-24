#include "util.h"
#include <iostream>


ChessPosition boardTransform(int x, int y) {
    // 窗口坐标以左上角为原点
    return ChessPosition(x * board::iHatX + y * board::jHatX + board::OriginX, -(x * board::iHatY + y * board::jHatY) + board::OriginY);
}

ChessPosition boardTransform(ChessPosition var,int alpha) {
    ChessPosition rotate=rotateCounterclockwise(var,alpha);
    double x = rotate.first, y = rotate.second;
    return boardTransform(x, y);
}

QString getColorName(int color) {
    QString colorName;
    switch (color) {
        case 0:
            colorName = "hint";
            break;
        case 1:
            colorName = "red";
            break;
        case 2:
            colorName = "blue";
            break;
        case 3:
            colorName = "yellow";
            break;
        case 4:
            colorName = "orange";
            break;
        case 5:
            colorName = "pink";
            break;
        case 6:
            colorName = "purple";
            break;
    }
    return colorName;
}

bool valid_check(int num)
{
    switch(num) {
        case 2:
        case 3:
        case 4:
        case 6:
            return true;
    }
    return false;
}

bool isCollinear(const ChessPosition& arg1, const ChessPosition& arg2) {
    int x(arg1.first), y(arg1.second), xx(arg2.first), yy(arg2.second);
    return x == xx || y == yy || x+y == xx+yy;
}

bool isCollinear(const ChessPosition& arg1, const ChessPosition& arg2, const ChessPosition& arg3) {
    int x = arg2.first - arg1.first, y = arg2.second - arg1.second, xx = arg3.first - arg2.first, yy = arg3.second - arg2.second;
    //    std::cerr<<x*yy-y*xx<<std::endl;
    return isCollinear(arg1, arg2) & isCollinear(arg2, arg3) & (x * yy == xx * y) & ((x * xx > 0) | (y * yy > 0));
}

bool isWithinBoundary(const ChessPosition& arg) {
    return (arg.first+arg.second <= board::boundary && arg.first >= -board::boundary && arg.second >= -board::boundary) ||
           (arg.first <= board::boundary && arg.second <= board::boundary && arg.first+arg.second >= -board::boundary);
}

bool isNeighbor(const ChessPosition& arg1, const ChessPosition& arg2) {
    return std::max(abs(arg1.first - arg2.first), abs(arg1.second - arg2.second)) <= 1;
}

ChessPosition jumpOver(const ChessPosition& arg1, const ChessPosition& arg2) {
    return ChessPosition(2 * arg2.first - arg1.first, 2 * arg2.second - arg1.second);
}

void outChessPosition(ChessPosition p) {
    std::cout << p.first << " " << p.second << std::endl;
}

int getSpawn(QString ID){
    return (int)*ID.toLatin1().data()-'A'+1;
}

int getTarget(QString ID){
    int ret=(int)*ID.toLatin1().data()-'A'+4;
    return ret>6?ret-6:ret;
}
QString getQColor(int color) {
    switch (color) {
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

QString getID(int x)
{
    return QString((char)(x+'A'-1));
}

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

int mod6Add(int x,int t){
    int ret=x+t;
    if(ret<=0) ret+=6;
    if(ret>6) ret-=6;
    return ret;
}
