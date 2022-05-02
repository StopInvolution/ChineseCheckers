#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <utility>
#include "settings.h"

// 棋盘坐标转换到显示坐标
ChessPosition boardTransform(ChessPosition var);
ChessPosition boardTransform(int x, int y);

// 输入编号返回颜色名字
QString getColorName(int color);

// 输入颜色编号返回颜色对应的 QColor 用于给文字配色
QString getQColor(int color);

// 是否共线
bool isCollinear(const ChessPosition& arg1, const ChessPosition& arg2);

// 返回 arg1 arg2 arg3 是否按顺序共线
bool isCollinear(const ChessPosition& arg1, const ChessPosition& arg2, const ChessPosition& arg3);

bool isWithinBoundary(const ChessPosition& arg);

// 是否相邻
bool isNeighbor(const ChessPosition& arg1, const ChessPosition& arg2);

// 返回 arg1 到 arg2 倍长线的终点
ChessPosition jumpOver(const ChessPosition& arg1, const ChessPosition& arg2);

// 输出 ChessPosition 类
void outChessPostion(ChessPosition p);

#endif  // UTIL_H
