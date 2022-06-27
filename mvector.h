#ifndef MVECTOR_H
#define MVECTOR_H

#include <QObject>
#include <QVector>
#include <QMutex>

/**
 * @brief The MVector class 是线程安全的Vector容器
 */
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
