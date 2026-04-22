#ifndef SLL_H
#define SLL_H

template <typename T>
struct Node
{
    T data;
    Node *next;
    Node(const T &d)
    {
        data = d;
        next = nullptr;
    }
};

template <typename T>
class LinkedList
{
private:
    Node<T> *head;

public:
    LinkedList()
    {
        head = nullptr;
    }
    ~LinkedList()
    {
        Node<T> *cur = head;
        while (cur)
        {
            Node<T> *tmp = cur;
            cur = cur->next;
            delete tmp;
        }
    }
    void push_front(const T &val)
    {
        Node<T> *n = new Node<T>(val);
        n->next = head;
        head = n;
    }

    template <typename P>
    bool remove(const P &pred)
    {
        Node<T> *cur = head;
        Node<T> *prev = nullptr;
        while (cur)
        {
            if (pred(cur->data))
            {
                if (prev)
                    prev->next = cur->next;
                else
                    head = cur->next;
                delete cur;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    template <typename P>
    T *find(const P &pred)
    {
        Node<T> *cur = head;
        while (cur)
        {
            if (pred(cur->data))
                return &cur->data;
            cur = cur->next;
        }
        return nullptr;
    }

    Node<T> *getHead() const
    {
        return head;
    }
};

#endif