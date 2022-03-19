#ifndef UTIL_H
#define UTIL_H

#include <utility>
#include "settings.h"
#include <QString>

// 棋盘坐标转换到显示坐标
ChessPostion boardTransform(ChessPostion var);
ChessPostion boardTransform(int x,int y);

QString getColorName(int color);
QString getQColor(int color);

// 是否共线
bool isCollinear(const ChessPostion &arg1,const ChessPostion &arg2);

// 返回 arg1 arg2 arg3 是否按顺序共线
bool isCollinear(const ChessPostion &arg1, const ChessPostion &arg2, const ChessPostion &arg3);

bool isWithinBoundary(const ChessPostion &arg);

// 返回 arg1 到 arg2 倍长线的终点
ChessPostion jumpOver(const ChessPostion &arg1, const ChessPostion &arg2);

// 输出 ChessPosition 类
void outChessPostion(ChessPostion p);

// 判断是否在某个大本营内，中间返回0，否则返回对应序号
int getZone(ChessPostion pst);
#endif // UTIL_H
