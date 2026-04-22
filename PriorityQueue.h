#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <vector>
#include <algorithm>
#include "Task.h"

struct CompareTask
{
    bool operator()(const Task &a, const Task &b)
    {
        if (a.getPriority() == b.getPriority())
            return a.getPriority() > b.getPriority();
        return a.getDL() > b.getDL();
    }
};

class PriorityQueue
{
    private:
    std::vector<Task> tasks;
    CompareTask cpt;

public:
    void enqueue(const Task &t)
    {
        tasks.push_back(t);
    }
    bool empty() const
    {
        return tasks.empty();
    }
    Task dequeue()
    {
        std::sort(tasks.begin(), tasks.end(), CompareTask());
        Task t = tasks.back();
        tasks.pop_back();
        return t;
    }
};

#endif