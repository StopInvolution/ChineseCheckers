#ifndef AGENT_H
#define AGENT_H
#include "settings.h"
#include <QList>
#include <QString>

pcc calculate(QVector<AlgoPlayer> vec);
Agent_algorithm *get_agent_algorithm(QString func);

#endif // AGENT_H
