#include <iostream>
template <typename T>
struct Node
{
    T data;
    Node *prev;
    Node *next;
    Node(T data) : data(data)
    {
        prev = nullptr;
        next = nullptr;
    }
};
template <typename T>
class LinkedList
{
    Node<T> *first;
    Node<T> *last;
    int len;
    void copyList(LinkedList const &other); //
    void deleteList();                      //

public:
    LinkedList();                                   //
    LinkedList(LinkedList const &other);            //
    ~LinkedList();                                  //
    LinkedList &operator=(LinkedList const &other); //

    void pushBack(T const &x);            //
    void pushFront(T const &x);           //
    void insertAt(T const &x, int index); //

    T getAt(int index);    //
    T popBack();           //
    T popFront();          //
    T removeAt(int index); //

    void print();                         //
    bool empty();                         //
    int size();                           //
    void reverse();                       //
    void append(LinkedList const &other); //
    void concat(LinkedList const &other); //
};
template <typename T>
LinkedList<T>::LinkedList()
{
    first = last = nullptr;
    len = 0;
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
void LinkedList<T>::copyList(LinkedList<T> const &other)
{
    first = last = nullptr;
    len = 0;
    Node<T> *p = other.first;
    while (p)
    {
        pushBack(p->data);
        p = p->next;
    }
}
template <typename T>
void LinkedList<T>::deleteList()
{
    Node<T> *p = first;
    while (p)
    {
        Node<T> *temp = p;
        p = p->next;
        delete temp;
    }
    len = 0;
}
template <typename T>
void LinkedList<T>::pushBack(T const &x)
{
    Node<T> *p = new Node<T>(x);
    if (empty())
    {
        first = last = p;
        ++len;
    }
    else
    {
        p->prev = last;
        last->next = p;
        last = p;
        ++len;
    }
}
template <typename T>
void LinkedList<T>::pushFront(T const &x)
{
    Node<T> *p = new Node<T>(x);
    if (empty())
    {
        first = last = p;
        ++len;
    }
    else
    {
        p->next = first;
        first->prev = p;
        first = p;
        ++len;
    }
}
template <typename T>
void LinkedList<T>::insertAt(T const &x, int index)
{
    if (index > len)
    {
        return;
    }
    if (index == 0)
    {
        pushFront(x);
    }
    else if (index == len )
    {
        pushBack(x);
    }
    else
    {
        Node<T> *p = new Node<T>(x);
        Node<T> *temp = first;
        for (int i = 0; i < index; i++)
        {
            temp = temp->next;
        }
        p->prev = temp;
        p->next = temp->next;
        temp->next = p;
        ++len;
    }
}
template <typename T>
T LinkedList<T>::getAt(int index)
{
    if (empty() || index >= len || index < 0)
        return T();
    if (index == 0)
        return first->data;
    else if (index == len - 1)
        return last->data;
    else
    {
        Node<T> *p = first;
        for (int i = 0; i < index; i++)
        {
            p = p->next;
        }
        return p->data;
    }
}
template <typename T>
T LinkedList<T>::popBack()
{
    if (empty())
        return T();
    if (first == last)
    {
        T x = last->data;
        delete last;
        --len;
        return x;
    }
    else
    {
        Node<T> *temp = last;
        T x = temp->data;
        last = temp->prev;
        last->next = nullptr;
        delete temp;
        --len;
        return x;
    }
}
template <typename T>
T LinkedList<T>::popFront()
{
    if (empty())
        return T();
    if (first == last)
    {
        T x = first->data;
        delete first;
        --len;
        return x;
    }
    else
    {
        Node<T> *temp = first;
        T x = temp->data;
        first = temp->next;
        first->prev = nullptr;
        delete temp;
        --len;
        return x;
    }
}
template <typename T>
T LinkedList<T>::removeAt(int index)
{
    if (empty() || index < 0 || index >= len)
        return T();
    if (index == 0)
        return popFront();
    if (index == len - 1)
        return popBack();
    Node<T> *rem = first;
    for (int i = 0; i < index; i++)
    {
        rem = rem->next;
    }
    T x = rem->data;
    rem->next->prev = rem->prev;
    rem->prev->next = rem->next;
    delete rem;
    --len;
    return x;
}
template <typename T>
bool LinkedList<T>::empty()
{
    return len == 0;
}
template <typename T>
int LinkedList<T>::size()
{
    return len;
}
template <typename T>
void LinkedList<T>::print()
{
    if (empty())
        std::cout << "empty\n";
    Node<T> *p = first;
    while (p)
    {
        std::cout << p->data << " ";
        p = p->next;
    }
    std::cout << std::endl;
}
template <typename T>
void LinkedList<T>::reverse()
{
    if (empty())
        return;
    Node<T> *temp = first;
    first = last;
    last = temp;
    Node<T> *next, *q;
    while (temp)
    {
        next = temp->next;
        q = temp->next;
        temp->next = temp->prev;
        temp->prev = q;
        temp = next;
    }
}
template <typename T>
void LinkedList<T>::append(LinkedList<T> const &other)
{
    Node<T> *p = other.first;
    while (p)
    {
        pushBack(p->data);
        p = p->next;
    }
}
template <typename T>
void LinkedList<T>::concat(LinkedList<T> const &other)
{
    last->next = other.first;
    other.first->prev = last;
    last = other.last;
}
