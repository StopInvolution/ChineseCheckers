#ifndef NETWORKUTIL_H
#define NETWORKUTIL_H
#include "settings.h"
#include <vector>
#include <QString>
void loadChessPosition(std::vector<ChessPosition>& vec,QString data);
std::vector<ChessPosition> loadChessPosition(QString data);
#endif // NETWORKUTIL_H
