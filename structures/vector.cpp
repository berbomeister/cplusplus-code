#include <iostream>
int closestPowerOfTwo(int n)
{
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}
template <typename T>
class Vector
{
    T *arr;
    int capacity;
    int count;
    void copyVector(Vector<T> const &other); //
    void deleteVector();                     //
    void Resize(int newCapacity);            //

public:
    Vector();                                     //
    Vector(int capacity);                         //
    Vector(Vector<T> const &other);               //
    Vector<T> &operator=(Vector<T> const &other); //
    ~Vector();                                    //

    void pushBack(T const &x);            //
    void pushFront(T const &x);           //
    void insertAt(T const &x, int index); //

    T getAt(int index);          //
    T &operator[](size_t index); //

    T popBack();           //
    T popFront();          //
    T removeAt(int index); //
    size_t getSize() const;
    void print() const;
    bool empty() const;

};
template <typename T>
void Vector<T>::copyVector(Vector<T> const &other)
{
    this->capacity = other.capacity;
    this->count = other.count;
    arr = new T[capacity];
    for (int i = 0; i < capacity; i++)
    {
        arr[i] = other.arr[i];
    }
}
template <typename T>
void Vector<T>::deleteVector()
{
    delete[] arr;
    count = 0;
}
template <typename T>
void Vector<T>::Resize(int newCapacity)
{
    T *temp = new T[newCapacity];
    for (int i = 0; i < capacity; i++)
    {
        temp[i] = arr[i];
    }
    capacity = newCapacity;
    delete[] arr;
    arr = temp;
}
template <typename T>
Vector<T>::Vector() : Vector(4)
{
}
template <typename T>
Vector<T>::Vector(int num)
{
    capacity = closestPowerOfTwo(num);
    arr = new T[capacity];
    count = 0;
}
template <typename T>
Vector<T>::Vector(Vector<T> const &other)
{
    copyVector(other);
}
template <typename T>
Vector<T> &Vector<T>::operator=(Vector<T> const &other)
{
    if (this != other)
    {
        deleteVector();
        copyVector(other);
    }
    return *this;
}
template <typename T>
Vector<T>::~Vector()
{
    deleteVector();
}
template <typename T>
void Vector<T>::pushBack(T const &x)
{
    if (count == capacity)
    {
        Resize(capacity * 2);
    }
    arr[count++] = x;
}
template <typename T>
void Vector<T>::pushFront(T const &x)
{
    if (count == capacity)
    {
        Resize(capacity * 2);
    }
    for (int i = count; i >= 1; i--)
    {
        arr[i] = arr[i - 1];
    }
    arr[0] = x;
    count++;
}
template <typename T>
void Vector<T>::insertAt(T const &x, int index)
{
    if (count == capacity)
    {
        Resize(capacity * 2);
    }
    for (int i = count; i > index; i--)
    {
        arr[i] = arr[i - 1];
    }
    arr[index] = x;
    count++;
}
template <typename T>
T Vector<T>::getAt(int index)
{
    if (index < 0 || index >= count)
    {
        return T();
    }
    return arr[index];
}
template <typename T>
T &Vector<T>::operator[](size_t index)
{
    return getAt(index);
}
template <typename T>
T Vector<T>::popBack()
{
    if (empty())
    {
        return T();
    }
    T x = arr[--count];
    if (count < capacity / 2)
        Resize(capacity / 2);
    return x;
}
template <typename T>
T Vector<T>::popFront()
{
    if (empty())
    {
        return T();
    }
    T x = arr[0];
    for (int i = 1; i < count; i++)
    {
        arr[i - 1] = arr[i];
    }
    if (--count < capacity / 2)
        Resize(capacity / 2);
    return x;
}
template <typename T>
T Vector<T>::removeAt(int index)
{
    if (empty() || index < 0 || index >= count)
        return T();
    T x = arr[index];
    for (int i = index; i < count - 1; i++)
    {
        arr[i] = arr[i + 1];
    }
    if (--count < capacity / 2)
        Resize(capacity / 2);
    return x;
}
template <typename T>
size_t Vector<T>::getSize() const
{
    return count;
}
template <typename T>
void Vector<T>::print() const
{
    for (int i = 0; i < count; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}
template <typename T>
bool Vector<T>::empty() const
{
    return count == 0;
}
int main()
{
    return 0;
}