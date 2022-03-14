#ifndef SETTINGS_H
#define SETTINGS_H

#include <utility>

typedef std::pair<double,double> ChessPostion;


namespace MarbleKind{
    const int hint=0;
    const int red=1;
    const int blue=2;
    const int yellow=3;
    const int white=4;
    const int pink=5;
    const int purple=6;

}//namesapce Color


namespace MarbleInfo{
    const int r=10;
}//namespace Marble


namespace Board{
    const double iHatX=1;
    const double iHatY=0;
    const double jHatX=-0.5;
    const double jHatY=-0.866025404;
    const int OriginX=471;
    const int OriginY=312;
    const ChessPostion iHat(iHatX,iHatY);
    const ChessPostion jHat(jHatX,jHatY);
    const ChessPostion OriginPoint(OriginX,OriginY);

}//namespace Board
#endif // SETTINGS_H
