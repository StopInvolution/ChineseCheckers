#include "networkUtil.h"
#include <QStringList>
#include <QDebug>
void loadChessPosition(std::vector<ChessPosition> &vec, QString data)
{
    QStringList list=data.split(' ');
    qDebug()<<list;

    for(int i=0;i<list.size();i+=2){
        vec.push_back(ChessPosition(list[i].toInt(),list[i+1].toInt()));
    }
}

std::vector<ChessPosition> loadChessPosition(QString data)
{
    QStringList list=data.split(' ');
    std::vector<ChessPosition> vec;
    for(int i=0;i<list.size();i+=2){
        vec.push_back(ChessPosition(list[i*2].toInt(),list[i*2+1].toInt()));
    }
    return vec;
}

void loadPlayerName(std::vector<QString> &vec, QString data)
{
    QStringList list=data.split(' ');
    for(int i=0;i<list.size();i++){
        vec.push_back(list[i]);
    }
}

void loadReady(std::vector<bool> &vec, QString data)
{
    for(int i=0;i<data.size();i++){
        vec.push_back(data[i]=='1');
    }
}

void loadPR(std::vector<pss> &vec, QString &data1, QString &data2)
{
    std::vector<QString> v1;
    std::vector<QString> v2;
    loadPlayerName(v1,data1);
    loadPlayerName(v2,data2);
    for(size_t i=0;i<v1.size();i++){
        vec.push_back(pss(v1[i],v2[i]));
    }
}
