#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <utility>
#include "networkdata.h"
#include "settings.h"

/**
 * @brief boardTransform 棋盘坐标转换到显示坐标，可以带旋转
 * @param var 棋盘坐标
 * @param alpha 旋转角度
 * @return
 */
ChessPosition boardTransform(ChessPosition var,int alpha=0);
ChessPosition boardTransform(int x, int y);

/**
 * @brief getColorName 编号对应的颜色名字
 * @param color 编号
 * @return 颜色名字，如"green"
 */
QString getColorName(int color);

/**
 * @brief getQColor 颜色编号对应的CSS格式颜色
 * @param color 颜色编号
 * @return CSS格式颜色
 */
QString getQColor(int color);

/**
 * @brief isCollinear 是否共线
 * @param arg1 第1个点
 * @param arg2 第2个点
 * @return 是否共线
 */
bool isCollinear(const ChessPosition& arg1, const ChessPosition& arg2);

/**
 * @brief isCollinear 是否三点共线
 * @param arg1 第1个点
 * @param arg2 第2个点
 * @param arg3 第3个点
 * @return 是否共线
 */
bool isCollinear(const ChessPosition& arg1, const ChessPosition& arg2, const ChessPosition& arg3);

/**
 * @brief isWithinBoundary 是否在棋盘边界内
 * @param arg 坐标
 * @return 是否在边界内
 */
bool isWithinBoundary(const ChessPosition& arg);

/**
 * @brief isNeighbor 两个棋子是否相邻，一步能走到的相邻
 * @param arg1 第1个点
 * @param arg2 第2个点
 * @return 是否相邻
 */
bool isNeighbor(const ChessPosition& arg1, const ChessPosition& arg2);

/**
 * @brief jumpOver 计算倍长线的终点
 * @param arg1 起点
 * @param arg2 中点
 * @return 倍长线的终点
 */
ChessPosition jumpOver(const ChessPosition& arg1, const ChessPosition& arg2);

/**
 * @brief outChessPosition 临时输出ChessPosition
 * @param p chessPosition
 */
void outChessPosition(ChessPosition p);

/**
 * @brief getSpawn 返回位置编码对应的出生位置序号
 * @param ID 位置编码
 * @return 位置序号
 */
int getSpawn(QString ID);

/**
 * @brief getTarget 返回位置编码对应的终点位置序号
 * @param ID 位置编码
 * @return 位置序号
 */
int getTarget(QString ID);

/**
 * @brief getID 返回位置序号对应的位置编码
 * @param x 位置序号
 * @return 位置编码
 */
QString getID(int x);

/**
 * @brief valid_check check if the number of players is valid.
 * @return is valid
 */
bool valid_check(int);

ChessPosition rotateCounterclockwise (ChessPosition begin, int alpha);

int mod6Add(int x,int t);

QString convertToQStr(ERRCODE err);
#endif  // UTIL_H
