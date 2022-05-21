#include "settings.h"

AlgoPlayer::AlgoPlayer(int s)
{
    spawn=s;
    target=s+3>6?s-3:s+3;
    for(int i=0;i<10;i++){
        pst[i]=ChessPosition(board::spawnPst[s][i*2],board::spawnPst[s][i*2+1]);
    }
}
