#ifndef QUEUE_H
#define QUEUE_H

#include <list>

template <typename T>
class Queue
{
    std::list<T> data;

public:
    void enqueue(const T &val)
    {
        data.push_back(val);
    }
    void dequeue()
    {
        if (!data.empty())
            data.pop_front();
    }
    T &front()
    {
        return data.front();
    }
    bool empty() const
    {
        return data.empty();
    }
};

#endif