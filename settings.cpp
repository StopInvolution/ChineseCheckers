#include "settings.h"

AlgoPlayer::AlgoPlayer(int s)
{
    spawn=s;
    target=s+3>6?s-3:s+3;
    for(int i=0;i<10;i++){
        pst[i]=ChessPosition(board::spawnPst[s][i*2],board::spawnPst[s][i*2+1]);
    }
}

namespace Network {
//    QString ip[4] = {"127","0","0","1"};
//    int port = 8000;
//    double resTime=300;
//    int serverPort = 8000;
//    QString ip[4] = {"10","46","54","135"};
    QString ip[4] = {"10","46","20","137"};
    int port = 8007;
    double resTime=120;
    int serverPort = 8007;
}
