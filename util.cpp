#include "util.h"

ChessPostion boardTransform(ChessPostion var){
    double x=var.first,y=var.second;
    return ChessPostion(x*Board::iHatX+y*Board::jHatX+Board::OriginX,x*Board::iHatY+y*Board::jHatY+Board::OriginY);
}

ChessPostion boardTransform(double x,double y){
    return ChessPostion(x*Board::iHatX+y*Board::jHatX+Board::OriginX,x*Board::iHatY+y*Board::jHatY+Board::OriginY);
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
        colorName="white";
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
