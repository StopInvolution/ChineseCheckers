#ifndef AGENT_H
#define AGENT_H
#include "settings.h"
#include <QList>
#include <QString>

void printvec(QVector<AlgoPlayer> vec);

void printmap(int (&map)[17][17]);

void printdist();

int MapX (ChessPosition pst);

int MapY (ChessPosition pst);

int MapValue (ChessPosition pst, int (&map)[17][17]);

int DistValue (ChessPosition pst, int target);

ChessPosition ToChessPosition (int x, int y);

ChessPosition MoveTo (ChessPosition original, int moveId);

int MoveValue (ChessPosition begin, ChessPosition end, int target);

struct moveStruct { //记录一步移动的struct
    int beginNo; //moved chess' number in pst[]
    ChessPosition begin; //begin position
    ChessPosition end; //end position
    int target; //target area
    int value_dist; //move value (only dist)
    bool operator< (const moveStruct right) const {
        //if (target != right.target) //error
        //    while(1);
        //if (value != right.value)
            return value_dist < right.value_dist;
        //return DistValue(begin,target) < DistValue(right.begin, right.target); //落后的追上
    };
};

void executeMove (moveStruct &move, QVector<AlgoPlayer> &vec, int (&map)[17][17], int NowPlayer);

void reverseMove (moveStruct &move, QVector<AlgoPlayer> &vec, int (&map)[17][17], int NowPlayer);

bool hasWon (QVector<AlgoPlayer> &vec, int NowPlayer);

int calculateBack(AlgoPlayer &now);

pcc calculateGreedy(QVector<AlgoPlayer> vec);

int minimaxDfs(QVector<AlgoPlayer> &vec, int (&map)[17][17], bool (&vis)[17][17][10], int depth, double nodeValue, int oldBack, int oldPotential, int situation, double alpha, double beta, int MinimaxDepth);

pcc calculateMinimax(QVector<AlgoPlayer> vec, int referenceMinimaxDepth);

pcc calculateAuto(QVector<AlgoPlayer> vec);

pcc calculatePre(QVector<AlgoPlayer> vec);

Agent_algorithm *get_agent_algorithm(QString func);

#endif // AGENT_H
