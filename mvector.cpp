#include "mvector.h"

template<class T>
void MVector<T>::push_back(T t)
{
    this->lock();
    QVector<T>::push_back(t);
    this->unlock();
}

template<class T>
T &MVector<T>::at(size_t t)
{
    this->lock();
    return QVector<T>::at(t);
    this->unlock();
}

template<class T>
size_t MVector<T>::size()
{
    this->lock();
    return QVector<T>::size();
    this->unlock();
}
