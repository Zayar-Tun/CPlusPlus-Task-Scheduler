#include "Task.h"
#include "TaskList.h"
#include "Stack.h"
#include "Queue.h"
#include "PriorityQueue.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cctype>
#include <cmath>

using namespace std;

int precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

bool isOp(const string &s)
{
    return s == "+" || s == "-" || s == "*" || s == "/";
}

bool isNumberTkn(const string &s)
{
    if (s.empty())
        return false;
    int dots = 0;
    bool hasDigit = false;
    for (char c : s)
    {
        if (c == '.')
        {
            if (++dots > 1)
                return false;
        }
        else if (isdigit(static_cast<unsigned char>(c)))
        {
            hasDigit = true;
        }
        else
            return false;
    }
    return hasDigit;
}

// Replace "priority of <TaskID>" with the actual number
string resolveIdentifiers(const string &expr, TaskList &tl)
{
    istringstream iss(expr);
    ostringstream out;
    string tok;

    while (iss >> tok)
    {
        if (tok == "priority")
        {
            string of, taskID;
            if (!(iss >> of >> taskID) || of != "of")
            {
                throw runtime_error("Expected: 'priority of <TaskID>'");
            }
            Task *dep = tl.findTask(taskID);
            if (!dep)
                throw runtime_error("Unknown task " + taskID);
            out << dep->getPriority() << ' ';
        }
        else
        {
            out << tok << ' ';
        }
    }
    return out.str();
}

// Convert space-separated infix string to postfix tokens
vector<string> infixToPostfix(const string &expr)
{
    Stack<char> ops;
    vector<string> output;

    istringstream iss(expr);
    string tok;

    while (iss >> tok)
    {
        if (!tok.empty() && isNumberTkn(tok))
        {
            output.push_back(tok);
        }

        else if (isOp(tok) && tok.size() == 1)
        {
            // pop operators with >= precedence
            while (!ops.empty() && precedence(ops.top()) >= precedence(tok[0]) && ops.top() != '(')
            {
                output.push_back(string(1, ops.top()));
                ops.pop();
            }
            ops.push(tok[0]);
        }

        else if (tok == "(")
        {
            ops.push('(');
        }

        else if (tok == ")")
        {
            // pop until left parenthesis
            while (!ops.empty() && ops.top() != '(')
            {
                output.push_back(string(1, ops.top()));
                ops.pop();
            }

            if (ops.empty())
                throw runtime_error("Mismatched ')");

            ops.pop();
        }

        else
        {
            throw runtime_error("Unknown token in expression: " + tok);
        }
    }

    // pop any remaining operators
    while (!ops.empty())
    {
        if (ops.top() == '(')
            throw std::runtime_error("Mismatched '('");
        output.push_back(string(1, ops.top()));
        ops.pop();
    }

    if (output.empty())
        throw std::runtime_error("Empty expression");

    return output;
}

// Evaluate postfix tokens
double postfixTks(const vector<string> &pf)
{
    Stack<double> st;
    for (const auto &tok : pf)
    {
        if (!tok.empty() && isNumberTkn(tok))
        {
            st.push(stod(tok));
        }
        else
        {
            double b = st.top();
            st.pop();
            double a = st.top();
            st.pop();
            if (tok == "+")
                st.push(a + b);
            else if (tok == "-")
                st.push(a - b);
            else if (tok == "*")
                st.push(a * b);
            else if (tok == "/")
            {
                if (b == 0.0)
                    throw std::runtime_error("Division by zero in expression");
                st.push(a / b);
            }
            else
                throw runtime_error("Unknown operator in postfix: " + tok);
        }
    }
    return st.top();
}

// Compute priority from expression string
int computePriorityFromExpr(const string &expr, TaskList &tl)
{
    string resolved = resolveIdentifiers(expr, tl);
    auto pf = infixToPostfix(resolved);
    double d = postfixTks(pf);
    int p = static_cast<int>(std::round(d));
    if (p < 1)
        p = 1;
    if (p > 10)
        p = 10;
    return p;
}

void trimIn(string &s)
{
    auto l = s.find_first_not_of(" \t\r\n");
    auto r = s.find_last_not_of(" \t\r\n");
    if (l == string::npos)
    {
        s.clear();
        return;
    }
    s = s.substr(l, r - l + 1);
}

void addTask(TaskList &tl)
{
    string id, desc, dep, dlstr, pristr;

    cout << "ID: ";
    getline(cin, id);
    trimIn(id);
    cout << "Description: ";
    getline(cin, desc);
    trimIn(desc);
    cout << "Priority (number or expression): ";
    getline(cin, pristr);
    trimIn(pristr);
    cout << "Deadline (hours): ";
    getline(cin, dlstr);
    trimIn(dlstr);
    cout << "Dependency TaskID (empty if none): ";
    getline(cin, dep);
    trimIn(dep);

    if (id.empty())
    {
        cout << "ID cannot be empty. Task not added.\n";
        return;
    }
    if (tl.findTask(id))
    {
        cout << "Task with ID '" << id << "' already exists. Skipping.\n";
        return;
    }
    int deadline;
    try
    {
        deadline = stoi(dlstr);
        if (deadline < 0)
        {
            cout << "Deadline cannot be negative. Task not added.\n";
            return;
        }
    }
    catch (...)
    {
        cout << "Invalid deadline. Task not added.\n";
        return;
    }
    if (dep == id)
    {
        cout << "A task cannot depend on itself. Ignoring dependency.\n";
        dep.clear();
    }

    int pri;
    bool numeric = isNumberTkn(pristr);
    try
    {
        if (numeric)
        {
            double d = stod(pristr);
            pri = static_cast<int>(round(d));
        }
        else
            pri = computePriorityFromExpr(pristr, tl);
    }
    catch (const exception &e)
    {
        cout << "Invalid Priority (" << e.what()
             << "). Task not added.\n";
        return;
    }
    if (pri < 1)
    {
        cout << "Priority below 1. Clamping to 1.\n";
        pri = 1;
    }
    if (pri > 10)
    {
        cout << "Priority above 10. Clamping to 10.\n";
        pri = 10;
    }
    tl.insertTask(Task(id, desc, pri, deadline, dep));
}

void deleteTask(TaskList &tl)
{
    string id;
    cout << "Task ID to delete: ";
    getline(cin, id);
    cout << (tl.deleteTask(id) ? "Deleted.\n" : "Not found.\n");
}

void displayTasks(TaskList &tl)
{
    cout << "All tasks:\n";
    auto it = tl.iterator();
    while (it.hasNext())
    {
        Task t = it.next();
        t.display();
    }
}

void loadTasksFromFile(const string &filename, TaskList &tl)
{
    ifstream fin(filename);
    if (!fin)
    {
        cout << "Cannot open file.\n";
        return;
    }

    string line;
    while (getline(fin, line))
    {
        if (line.empty())
            continue;
        vector<string> f;
        f.reserve(6);
        string cell;
        istringstream ss(line);
        while (getline(ss, cell, ','))
            f.push_back(cell);

        if (f.size() < 4)
        {
            cout << "Bad line: " << line << "\n";
            continue;
        }

        string id = f[0];
        string desc = f[1];
        string pristr = f[2];
        int deadline = 0;
        try
        {
            deadline = stoi(f[3]);
        }
        catch (...)
        {
            deadline = 0;
        }
        string dep = (f.size() >= 5) ? f[4] : "";

        int pri = 5;
        bool numeric = isNumberTkn(pristr);
        try
        {
            if (numeric){
                double d = stod(pristr);
                pri = static_cast<int>(round(d));
            }
            else
                pri = computePriorityFromExpr(pristr, tl);
        }
        catch (const exception &e)
        {
            cout << "Note: could not evaluate '" << pristr << "' for " << id
                 << " (" << e.what() << "). Using 5.\n";
            pri = 5;
        }

        tl.insertTask(Task(id, desc, pri, deadline, dep));
    }
    cout << "Loaded.\n";
}

void saveCompletedToFile(const string &filename, Queue<Task> &completed)
{
    ofstream fout(filename);
    if (!fout)
    {
        cout << "Cannot open file to write.\n";
        return;
    }

    Queue<Task> tmp;
    while (!completed.empty())
    {
        Task &t = completed.front();
        // Format: ID,Description,Priority,Deadline,Dependency,,
        fout << t.getID() << "," << t.getDes() << "," << t.getPriority() << ","
             << t.getDL() << "," << t.getDepen() << ",,\n";
        tmp.enqueue(t);
        completed.dequeue();
    }
    // put back
    while (!tmp.empty())
    {
        completed.enqueue(tmp.front());
        tmp.dequeue();
    }

    cout << "Saved.\n";
}

// build a PQ of all tasks whose dependency is empty or already completed
void buildReadyPQ(TaskList &tl, const vector<string> &completedIDs, PriorityQueue &pq)
{
    auto it = tl.iterator();
    while (it.hasNext())
    {
        Task t = it.next();
        string dep = t.getDepen();
        bool ok = dep.empty() || find(completedIDs.begin(), completedIDs.end(), dep) != completedIDs.end();
        if (ok)
        {
            pq.enqueue(t);
        }
    }
}

// schedule next
void scheduleNext(TaskList &tl, vector<string> &completedIDs, Queue<Task> &completedQ)
{
    if (tl.hasCyclicDependencies())
    {
        cout << "Cycle detected. Resolve before scheduling.\n";
        return;
    }

    PriorityQueue pq;
    buildReadyPQ(tl, completedIDs, pq);

    if (pq.empty())
    {
        cout << "No ready tasks (maybe waiting on dependencies).\n";
        return;
    }

    Task next = pq.dequeue();
    cout << "Next to execute:\n";
    next.display();

    // mark completed
    completedQ.enqueue(next);
    completedIDs.push_back(next.getID());
    tl.deleteTask(next.getID());
}

void checkCycles(TaskList &tl)
{
    bool cyc = tl.hasCyclicDependencies();
    cout << (cyc ? "Cycle exists.\n" : "No cycles.\n");

    if (cyc)
    {
        string id;
        cout << "Enter a TaskID to see who depends on it: ";
        getline(cin, id);
        auto deps = tl.findDependentTasks(id);
        if (deps.empty())
            cout << "No dependents for " << id << ".\n";
        else
        {
            cout << "Dependents of " << id << ": ";
            for (auto &x : deps)
                cout << x << " ";
            cout << "\n";
        }
    }
}

int main()
{
    TaskList tl;
    Queue<Task> completedQ;      // finished tasks FIFO
    vector<string> completedIDs; // for quick "dependency satisfied?" checks

    while (true)
    {
        cout << "\nTask Scheduler\n"
             << "1. Add Task\n"
             << "2. Delete Task\n"
             << "3. Display All Tasks\n"
             << "4. Schedule Next Task\n"
             << "5. Check Cyclic Dependencies\n"
             << "6. Save to File\n"
             << "7. Load from File\n"
             << "8. Exit\n"
             << "Enter Choice: ";

        int op;
        if (!(cin >> op))
        {
            cout << "Invalid input. Please enter a number 1–8.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(10000, '\n');

        if (op == 8)
            break;

        switch (op)
        {
        case 1:
            addTask(tl);
            break;
        case 2:
            deleteTask(tl);
            break;
        case 3:
            displayTasks(tl);
            break;
        case 4:
            scheduleNext(tl, completedIDs, completedQ);
            break;
        case 5:
            checkCycles(tl);
            break;
        case 6:
        {
            string fn;
            cout << "Filename: ";
            getline(cin, fn);
            saveCompletedToFile(fn, completedQ);
            break;
        }
        case 7:
        {
            string fn;
            cout << "Filename: ";
            getline(cin, fn);
            loadTasksFromFile(fn, tl);
            break;
        }
        default:
            cout << "Invalid input. Please enter a number 1–8..\n";
        }
    }
    cout << "Bye!\n";
    return 0;
}