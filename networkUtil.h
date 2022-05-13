#ifndef NETWORKUTIL_H
#define NETWORKUTIL_H
#include "settings.h"
#include <vector>
#include <QString>
void loadPlayerName(std::vector<QString>& vec,QString data);
void loadReady(std::vector<bool>& vec,QString data);
void loadChessPosition(std::vector<ChessPosition>& vec,QString data);
std::vector<ChessPosition> loadChessPosition(QString data);
void loadPR(std::vector<pss> &vec,QString &data1,QString &data2);
#endif // NETWORKUTIL_H
