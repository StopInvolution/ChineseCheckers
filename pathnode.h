#ifndef PATHNODE_H
#define PATHNODE_H
#include "settings.h"

class PathNode
{
public:
    PathNode(ChessPostion _pst=ChessPostion(0,0), PathNode *_from=0);
    ChessPostion pst;
    PathNode *from;
};

#endif // PATHNODE_H
