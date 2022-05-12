#include "networkUtil.h"
#include <QStringList>
void loadChessPosition(std::vector<ChessPosition> &vec, QString data)
{
    QStringList list=data.split(' ');
    for(int i=0;i<list.size();i+=2){
        vec.push_back(ChessPosition(list[i*2].toInt(),list[i*2+1].toInt()));
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
