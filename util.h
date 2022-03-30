#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <utility>
#include "settings.h"

// 棋盘坐标转换到显示坐标
ChessPostion boardTransform(ChessPostion var);
ChessPostion boardTransform(int x, int y);

// 输入编号返回颜色名字
QString getColorName(int color);

// 输入颜色编号返回颜色对应的 QColor 用于给文字配色
QString getQColor(int color);

// 是否共线
bool isCollinear(const ChessPostion& arg1, const ChessPostion& arg2);

// 返回 arg1 arg2 arg3 是否按顺序共线
bool isCollinear(const ChessPostion& arg1, const ChessPostion& arg2, const ChessPostion& arg3);

bool isWithinBoundary(const ChessPostion& arg);

// 是否相邻
bool isNeighbor(const ChessPostion& arg1, const ChessPostion& arg2);

// 返回 arg1 到 arg2 倍长线的终点
ChessPostion jumpOver(const ChessPostion& arg1, const ChessPostion& arg2);

// 输出 ChessPosition 类
void outChessPostion(ChessPostion p);

#endif  // UTIL_H
