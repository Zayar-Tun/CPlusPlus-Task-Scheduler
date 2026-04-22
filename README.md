# C++ Task Scheduler

## Project Overview
This project is a menu-driven task scheduler simulation built in C++. It manages tasks with priorities, deadlines, and dependencies, and demonstrates the use of core data structures and algorithms in a practical scheduling system.

The program allows users to add, delete, display, and schedule tasks, check for cyclic dependencies, and save or load task data from a file. It was developed as a data structures assignment focused on applying linked lists, stacks, queues, priority queues, recursion, iterators, and file handling.

## Features
- Add a task
- Delete a task
- Display all tasks
- Schedule the next task
- Check cyclic dependencies
- Save tasks to a file
- Load tasks from a file
- Menu-driven console interface

## Data Structures and Concepts Used
This project applies several important data structures and concepts:

- **Singly Linked List (SLL)** for storing tasks
- **Iterator ADT** for traversing the task list
- **Priority Queue** for scheduling tasks by priority and deadline
- **Stack** for expression conversion and evaluation
- **Queue** for completed tasks
- **Recursion** for detecting cyclic dependencies and dependency-related processing
- **File handling** for saving and loading tasks

## Task Information
Each task includes:
- Task ID
- Description
- Priority
- Deadline
- Dependency Task ID

Priority can also be entered as an arithmetic expression, such as:

`2 + 3 * priority of T002`

## How It Works
The scheduler processes tasks based on:
- **higher priority first** (lower number means higher priority)
- **earlier deadlines first**
- **dependency rules**, so tasks cannot be scheduled until required tasks are completed

The program also checks for cyclic dependencies and prevents scheduling tasks that are part of a cycle.

## Files Included
- `main.cpp` — main program and menu
- `Task.h` — task class definition
- `TaskList.h` — task list handling
- `SLL.h` — singly linked list implementation
- `Stack.h` — stack implementation
- `Queue.h` — queue implementation
- `PriorityQueue.h` — priority queue implementation
- `README.md` — project documentation

## How to Compile and Run
Compile the program using a C++ compiler such as `g++`.

Example:

```bash
g++ main.cpp -o scheduler
./scheduler
```

## Menu Options
When the program runs, it displays a menu with options such as:
1. Add Task
2. Delete Task
3. Display All Tasks
4. Schedule Next Task
5. Check Cyclic Dependencies
6. Save to File
7. Load from File
8. Exit

## File Format
Tasks are saved in a CSV-like format such as:

```text
T001,Write Report,5,24,,,
T002,Research,2,48,,,
T003,Review Notes,4,12,T001,,
```
Saved task files can be loaded back into the program.

## Notes
- Priorities outside the valid range are clamped
- Cyclic dependencies are reported and not scheduled
- Completed tasks are saved separately
- Expressions should be space-separated when entered
- This project was developed according to assignment requirements and data structure constraints

## What I Learned
Through this project, I practiced:
- implementing linked lists and iterators
- using stacks for expression processing
- using queues and priority queues for scheduling
- applying recursion to dependency checking
- working with file input and output
- designing a menu-driven C++ program
- analyzing data structure trade-offs and algorithm behavior

## Author
Zayar Tun
