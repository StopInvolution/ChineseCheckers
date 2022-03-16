#ifndef UTIL_H
#define UTIL_H

#include <utility>
#include "settings.h"
#include <QString>

ChessPostion boardTransform(ChessPostion var);

ChessPostion boardTransform(int x,int y);

QString getColorName(int color);

bool isCollinear(const ChessPostion &arg1,const ChessPostion &arg2);

// 返回 arg1 arg2 arg3 是否按顺序共线
bool isCollinear(const ChessPostion &arg1, const ChessPostion &arg2, const ChessPostion &arg3);

bool isWithinBoundary(const ChessPostion &arg);

// 返回 arg1 到 arg2 倍长线的终点
ChessPostion jumpOver(const ChessPostion &arg1, const ChessPostion &arg2);

void outChessPostion(ChessPostion p);
#endif // UTIL_H
