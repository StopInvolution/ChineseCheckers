#ifndef NETWORKUTIL_H
#define NETWORKUTIL_H
#include "settings.h"
#include <vector>
#include <QString>
void loadPlayerName(QVector<QString>& vec,QString data);
void loadReady(QVector<bool>& vec,QString data);
void loadChessPosition(QVector<ChessPosition>& vec,QString data);
QVector<ChessPosition> loadChessPosition(QString data);
void loadPR(QVector<pss> &vec,QString &data1,QString &data2);
#endif // NETWORKUTIL_H
