#ifndef Linked_List
#define Linked_List

#include <iostream>
template <typename T>
struct Node
{
    T data;
    Node *next;
};
template <typename T>
class LinkedList
{
    Node<T> *first;
    Node<T> *last;
    void deleteList();                      //
    void copyList(LinkedList const &other); //
    int len;

public:
    LinkedList();                                   //
    LinkedList(LinkedList const &other);            //
    ~LinkedList();                                  //
    LinkedList &operator=(LinkedList const &other); //
    bool operator==(LinkedList<T> const &x)
    {
        return first == x.first;
    }
    T &operator[](int index)
    {
        return this->getAtIndex(index);
    }

    void addLast(T const &x);  //
    void addFirst(T const &x); //
    void push(T const &x);     //

    void remove(T const &x); //
    T removeFirst();         //
    T removeLast();          //
    T removeNode(Node<T> *p);

    void print(); //
    int size();   //
    bool empty(); //
    bool find(T const &x);

    void reverse();                       //
    void append(LinkedList const &other); //
    void concat(LinkedList const &other); //
    Node<T> *begin();
    Node<T> *end();
    T getAtIndex(int index); //
    void addAtIndex(T const &x, int index);
};
template <typename T>
bool LinkedList<T>::find(T const &x)
{
    Node<T> *p = begin();
    while (p)
    {
        if (p->data == x)
            return true;
        p = p->next;
    }
    return false;
}
template <typename T>
T LinkedList<T>::removeNode(Node<T> *p)
{
    if (p == first)
        return removeFirst();
    else
    {
        Node<T> *q = first;
        while (q->next != p)
        {
            q = q->next;
        }
        T x = p->data;
        q->next = q->next->next;
        delete p;
        return x;
    }
}
template <typename T>
void LinkedList<T>::remove(T const &x)
{
    if (empty())
        return;
    if (first == last && first->data == x)
    {
        removeFirst();
        return;
    }

    Node<T> *p = begin();
    if (p->data == x)
    {
        removeFirst();
        return;
    }
    else
    {
        while (p->next)
        {
            if (p->next->data == x)
            {
                Node<T> *temp = p->next;
                p->next = p->next->next;
                delete temp;
                --len;
            }
            p = p->next;
        }
    }
}
template <typename T>
Node<T> *LinkedList<T>::begin()
{
    return first;
}
template <typename T>
Node<T> *LinkedList<T>::end()
{
    return last;
}
template <typename T>
void LinkedList<T>::push(T const &x)
{
    addLast(x);
}
template <typename T>
LinkedList<T>::LinkedList()
{
    first = last = nullptr;
    len = 0;
}
template <typename T>
void LinkedList<T>::deleteList()
{
    len = 0;
    Node<T> *p = first;
    while (p)
    {
        Node<T> *prev = p;
        p = p->next;
        delete prev;
    }
}
template <typename T>
void LinkedList<T>::copyList(LinkedList<T> const &other)
{
    first = last = nullptr;
    len = 0;
    Node<T> *p = other.first;
    while (p)
    {
        addLast(p->data);
        p = p->next;
    }
}
template <typename T>
LinkedList<T>::LinkedList(LinkedList<T> const &other)
{
    copyList(other);
}
template <typename T>
LinkedList<T>::~LinkedList()
{
    deleteList();
}
template <typename T>
LinkedList<T> &LinkedList<T>::operator=(LinkedList<T> const &other)
{
    if (this != &other)
    {
        deleteList();
        copyList(other);
    }
    return *this;
}
template <typename T>
void LinkedList<T>::addLast(T const &x)
{
    Node<T> *p = new Node<T>;
    p->data = x;
    p->next = nullptr;
    if (empty())
    {
        first = last = p;
    }
    else
    {
        last->next = p;
        last = p;
    }
    ++len;
}
template <typename T>
void LinkedList<T>::addFirst(T const &x)
{
    Node<T> *p = new Node<T>;
    p->data = x;
    if (empty())
    {
        first = p;
        last = p;
    }
    else
    {
        p->next = first;
        first = p;
    }
    ++len;
}
template <typename T>
T LinkedList<T>::removeFirst()
{
    if (empty())
        return T();
    else if (first == last)
    {
        T inf = first->data;
        --len;
        delete first;
        first = last = nullptr;
        return inf;
    }
    else
    {
        --len;
        Node<T> *temp = first;
        T inf = first->data;
        first = first->next;
        delete temp;
        return inf;
    }
}
template <typename T>
T LinkedList<T>::removeLast()
{
    if (empty())
        return T();
    else if (first == last)
    {
        T inf = last->data;
        delete first;
        first = last = nullptr;
        return inf;
    }
    else
    {
        T inf = last->data;
        Node<T> *p = first;
        while (p->next != last)
        {
            p = p->next;
        }
        delete last;
        last = p;
        p->next = nullptr;
        --len;
        return inf;
    }
}
template <typename T>
void LinkedList<T>::print()
{
    Node<T> *p = first;
    while (p)
    {
        std::cout << p->data << " ";
        p = p->next;
    }
    std::cout << std::endl;
}
template <typename T>
int LinkedList<T>::size()
{
    return len;
}
template <typename T>
bool LinkedList<T>::empty()
{
    return len == 0;
}
template <typename T>
T LinkedList<T>::getAtIndex(int index)
{
    if (index > len)
        return T();
    Node<T> *p = first;
    for (int i = 0; i < index && p; i++)
    {
        p = p->next;
    }
    return p->data;
}
template <typename T>
void LinkedList<T>::reverse()
{
    if (empty())
    {
        return;
    }
    Node<T> *p, *q, *temp;
    p = first;
    temp = nullptr;
    q = first;
    first = last;
    last = q;
    while (p != first)
    {
        q = p->next;
        p->next = temp;
        temp = p;
        p = q;
    }
    p->next = temp;
}
template <typename T>
void LinkedList<T>::append(LinkedList const &other)
{
    Node<T> *p = other.first;
    while (p)
    {
        this->addLast(p->data);
        p = p->next;
    }
}
template <typename T>
void LinkedList<T>::concat(LinkedList const &other)
{
    last->next = other.first;
    last = other.last;
}
template <typename T>
LinkedList<T> merge(LinkedList<T> left, LinkedList<T> right)
{
    LinkedList<T> result;
    while (!left.empty())
    {
        result.push(left.removeFirst());
    }

    while (!right.empty())
    {
        result.push(right.removeFirst());
    }
    return result;
}
template <typename T>
LinkedList<T> sort(LinkedList<T> list)
{
    if (list.empty())
        return list;
    if (list.size() == 1)
        return list;

    T pivot = list.removeFirst();
    LinkedList<T> left, right;
    while (!list.empty())
    {
        T temp = list.removeFirst();
        if (temp < pivot)
            left.push(temp);
        else
            right.push(temp);
    }
    left.push(pivot);
    return merge(sort(left), sort(right));
}
#endif