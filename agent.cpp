#include "util.h"
#include <QDebug>
#include <QVector>

pcc calculate(QVector<AlgoPlayer> vec){
    qDebug()<<vec.size();
    return pcc(ChessPosition(-4,5),ChessPosition(-4,4));
}
