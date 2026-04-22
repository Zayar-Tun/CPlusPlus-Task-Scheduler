#ifndef TASK_H
#define TASK_H
#include <iostream>
#include <string>
using namespace std;

class Task
{
private:
    string taskID, description, dependency;
    int priority, deadline;

public:
    Task(string id = "", string desc = "", int pri = 10, int dl = 0, string depen = "")
    {
        taskID = id;
        description = desc;
        priority = pri;
        deadline = dl;
        dependency = depen;
    }
    const string getID() const
    {
        return taskID;
    }
    const string getDes() const
    {
        return description;
    }
    int getPriority() const
    {
        return priority;
    }
    int getDL() const
    {
        return deadline;
    }
    const string getDepen() const
    {
        return dependency;
    }
    void display() const
    {
        cout << "\n" << taskID << "\n"
             << description << "\n[Priority: " << priority << ", Deadline: " << deadline << ", Dependency: " << dependency << "]\n\n";
    }
};

#endif