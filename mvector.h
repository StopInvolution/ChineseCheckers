#ifndef MVECTOR_H
#define MVECTOR_H

#include <QObject>
#include <QVector>
#include <QMutex>

template <class T>
class MVector:QVector<T>,QMutex
{
public:
    using QVector<T>::QVector;
    void push_back(T t);
    T& at(size_t t);
    size_t size();
};

#endif // MVECTOR_H
