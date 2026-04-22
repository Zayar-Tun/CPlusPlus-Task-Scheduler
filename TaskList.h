#ifndef TASKLIST_H
#define TASKLIST_H

#include <string>
#include <vector>
#include "Task.h"
#include "SLL.h"
using namespace std;

class TaskList
{
    LinkedList<Task> list;

public:
    void insertTask(const Task &task)
    {
        if (findTask(task.getID()) != nullptr) {
        cout << "Task with ID " << task.getID() << " already exists. Skipping.\n";
        return;
    }
        list.push_front(task);
        cout << "Added.\n";
    }

    bool deleteTask(const string &taskID)
    {
        return list.remove([&](const Task &t)
                           { return t.getID() == taskID; });
    }

    Task *findTask(const string &taskID)
    {
        return list.find([&](Task &t)
                         { return t.getID() == taskID; });
    }

    class TaskIterator
    {
        Node<Task> *current;
        Node<Task> *start;

    public:
        TaskIterator(Node<Task> *head)
        {
            current = head;
            start = head;
        }
        bool hasNext() const
        {
            return current != nullptr;
        }
        Task next()
        {
            Task out = current->data;
            current = current->next;
            return out;
        }
        void reset() { current = start; }
    };

    TaskIterator iterator() const
    {
        return TaskIterator(list.getHead());
    }

    bool hasCyclicDependencies()
    {
        Node<Task> *cur = list.getHead();
        while (cur)
        {
            vector<string> path; // keeps track of chain
            if (checkCycle(cur->data.getID(), path))
                return true;
            cur = cur->next;
        }
        return false;
    }

    vector<string> findDependentTasks(const string &taskID)
    {
        vector<string> result;
        vector<string> seen;
        collectDependents(taskID, result, seen);
        return result;
    }

private:
    // find task by ID helper
    Task *findTaskByID(const string &id)
    {
        Node<Task> *cur = list.getHead();
        while (cur)
        {
            if (cur->data.getID() == id)
                return &cur->data;
            cur = cur->next;
        }
        return nullptr;
    }

    // cycle detection helper
    bool checkCycle(const string &id, vector<string> &path)
    {
        if (id.empty())
            return false;

        // if ID is already in path, cycle exists
        for (auto &seen : path)
        {
            if (seen == id)
            {
                cout << "Cycle found along: ";
                for (auto &p : path)
                    cout << p << " -> ";
                cout << id << "\n";
                return true;
            }
        }

        path.push_back(id);

        Task *t = findTaskByID(id);
        if (t)
        {
            if (checkCycle(t->getDepen(), path))
                return true;
        }

        path.pop_back();
        return false;
    }

    // collect dependents helper
    void collectDependents(const string &id, vector<string> &out, vector<string> &seen)
    {
        if (find(seen.begin(), seen.end(), id) != seen.end())
            return;
        seen.push_back(id);

        Node<Task> *cur = list.getHead();
        while (cur)
        {
            if (cur->data.getDepen() == id)
            {
                const string &childID = cur->data.getID();
                // avoid duplicates in out
                if (find(out.begin(), out.end(), childID) == out.end())
                    out.push_back(childID);

                collectDependents(childID, out, seen);
            }
            cur = cur->next;
        }
    }
};

#endif

/*
Reference: https://www.gamedev.tv/articles/intro-to-lambda-expressions-ii-c
https://learn.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp?view=msvc-170
*/