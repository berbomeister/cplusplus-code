#include <iostream>

template <typename T>
struct StackElement
{
    T data;
    StackElement *next;
};

template <typename T>
class Stack
{
    StackElement<T> *topPtr;
    void copy(Stack const &other); //
    void erase();                  //
public:
    Stack();                              //
    ~Stack();                             //
    Stack(Stack const &other);            //
    Stack &operator=(Stack const &other); //
    bool empty();                         //
    void push(T const &x);                //
    T pop();                              //
    T const &top() const;                 //
};
template <typename T>
Stack<T>::Stack()
{
    topPtr = nullptr;
}
template <typename T>
void Stack<T>::erase()
{
    while (topPtr != nullptr)
    {
        StackElement<T> *temp = topPtr->next;
        delete topPtr;
        topPtr = temp;
    }
    delete topPtr;
}
template <typename T>
Stack<T>::~Stack()
{
    this->erase();
}
template <typename T>
bool Stack<T>::empty()
{
    return topPtr == nullptr;
}
template <typename T>
void Stack<T>::push(T const &x)
{
    StackElement<T> *temp = new StackElement<T>;
    temp->data = x;
    temp->next = topPtr;
    topPtr = temp;
}
template <typename T>
T Stack<T>::pop()
{
    if (empty())
        return T();
    T data = topPtr->data;
    StackElement<T> *temp = topPtr->next;
    delete topPtr;
    topPtr = temp;
    return data;
}
template <typename T>
T const &Stack<T>::top() const
{
    if (empty())
        return T();
    return topPtr->data;
}
template <typename T>
void Stack<T>::copy(Stack const &other)
{
    topPtr = nullptr;
    if (other.empty())
        return;
    StackElement<T> *p = other.topPtr;
    Stack<T> temp;
    while (p != nullptr)
    {
        temp.push(p->data);
        p = p->next;
    }
    while (!temp.empty())
    {
        push(temp.pop());
    }
}
template <typename T>
Stack<T>::Stack(Stack const &other)
{
    copy(other);
}
template <typename T>
Stack<T> &Stack<T>::operator=(Stack const &other)
{
    if (this != &other)
    {
        erase();
        copy(other);
    }

    return *this;
}
int main()
{
    Stack<int> container;
    for (int i = 0; i < 10; i++)
    {
        container.push(i);
    }
    for (int i = 0; i < 10; i++)
    {
        std::cout << container.pop() << " ";
    }
    return 0;
}