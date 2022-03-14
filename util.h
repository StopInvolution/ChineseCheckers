#ifndef UTIL_H
#define UTIL_H

#include <utility>
#include "settings.h"
#include <QString>

ChessPostion boardTransform(ChessPostion var);

ChessPostion boardTransform(double x,double y);

QString getColorName(int color);

#endif // UTIL_H
