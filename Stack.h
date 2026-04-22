#ifndef STACK_H
#define STACK_H

#include <vector>
#include <stdexcept>

template <typename T>
class Stack
{
    std::vector<T> data;

public:
    void push(const T &val)
    {
        data.push_back(val);
    }
    void pop()
    {
        if (!data.empty())
            data.pop_back();
    }
    T &top()
    {
        if (data.empty())
            throw std::runtime_error("Stack empty");
        return data.back();
    }
    bool empty() const
    {
        return data.empty();
    }
};

#endif