#ifndef BIN_TREE
#define BIN_TREE

#include <iostream>
#include <cassert>
template <typename T>
struct Node
{
    T data;
    Node<T> *left;
    Node<T> *right;
    Node() {}
    Node(T const &x, Node<T> *left = nullptr, Node<T> *right = nullptr) : data(x), left(left), right(right) {}
};
template <typename T>
class BinaryTree;

template <typename T>
class Iterator
{
    friend class BinaryTree<T>;
    Node<T> **ptr;

public:
    Iterator() : ptr(nullptr) {}
    Iterator(Node<T> *&node) : ptr(&node) {}
    bool valid() const
    {
        return ptr != nullptr && *ptr != nullptr;
    }
    Iterator<T> left() const
    {
        if (!valid())
            return Iterator<T>();
        return Iterator((*ptr)->left);
    }
    Iterator<T> right() const
    {
        if (!valid())
            return Iterator<T>();
        return Iterator((*ptr)->right);
    }
    T const &get() const
    {
        return (*ptr)->data;
    }
    T &get()
    {
        return (*ptr)->data;
    }
    operator bool() const
    {
        return valid();
    }
    Iterator<T> operator++()
    {
        return (*this = right());
    }
    Iterator<T> operator++(int)
    {
        Iterator<T> curr = *this;
        ++(*this);
        return curr;
    }
    Iterator<T> operator--()
    {
        return (*this = left());
    }
    Iterator<T> operator--(int)
    {
        Iterator<T> curr = *this;
        --(*this);
        return curr;
    }
    T &operator*()
    {
        return get();
    }
    bool operator==(Iterator<T> const &other)
    {
        return (!valid() && !other.valid()) || (valid() && other.valid() && *ptr = *other.ptr);
    }
    bool operator!=(Iterator<T> const &other)
    {
        return !(*this == other);
    }
};
template <typename T>
class BinaryTree
{
    Node<T> *root;
    void printHelper(std::ostream &out, Node<T> *pos); //
    void erase(Iterator<T> pos);                       //
    Iterator<T> copy(Iterator<T> pos);

public:
    BinaryTree();          //
    BinaryTree(const T &); //
    BinaryTree(BinaryTree &bt) : root(nullptr)
    {
        copy(bt.rootPos());
    }
    BinaryTree(BinaryTree &&bt) : root(nullptr)
    {
        assignFrom(rootPos(), bt.rootPos());
    }

    BinaryTree &operator=(BinaryTree &bt)
    {
        if (this != &bt)
        {
            erase(rootPos());
            copy(bt.rootPos());
        }
        return *this;
    }

    BinaryTree &operator=(BinaryTree &&bt)
    {
        if (this != &bt)
        {
            assignFrom(rootPos(), bt.rootPos());
        }
        return *this;
    }

    ~BinaryTree()
    {
        erase(rootPos());
    }

    Iterator<T> rootPos()
    {
        return Iterator<T>(root);
    }

    bool empty() const
    {
        return root == nullptr;
    }
    BinaryTree<T> &addElement(const char *, const T &); //

    void printTree(std::ostream &);

    void assignFrom(Iterator<T> from, Iterator<T> to)
    {
        BinaryTreeNode<T> *save = *from.ptr;
        *from.ptr = *to.ptr;
        *to.ptr = nullptr;
        erase(Iterator<T>(save));
    }
    BinTree<T> &addElement(const char *, const T &);
};
template <typename T>
BinaryTree<T>::BinaryTree() : root(nullptr) {}

template <typename T>
BinaryTree<T>::BinaryTree(T const &x)
{
    root = new Node<T>(x, nullptr, nullptr);
}
template <typename T>
BinaryTree<T> &BinaryTree<T>::addElement(const char *path, T const &x)
{
    if (strlen(trace) == 0)
    {
        assert(root == nullptr);
        root = new BinTreeNode<T>(x, nullptr, nullptr);
        return *this;
    }

    assert(root != nullptr);
    BinTreeNode<T> *current = root;

    for (int i = 0; i < strlen(trace) - 1; i++)
    {
        assert(trace[i] == 'L' || trace[i] == 'R');
        if (trace[i] == 'L')
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
        assert(current != nullptr);
    }
    assert(trace[strlen(trace) - 1] == 'L' || trace[strlen(trace) - 1] == 'R');
    if (trace[strlen(trace) - 1] == 'L')
    {
        assert(current->left == nullptr);
        current->left = new BinTreeNode<T>(x, nullptr, nullptr);
    }
    else
    {
        assert(current->right == nullptr);
        current->right = new BinTreeNode<T>(x, nullptr, nullptr);
    }

    return *this;
}

template <typename T>
void BinaryTree<T>::erase(Iterator<T> pos)
{
    if (pos)
    {
        erase(pos.left());
        erase(pos.right());
        delete *pos.ptr;
    }
}
template <typename T>
void BinaryTree<T>::printTree(std::ostream &out = std::cout) //
{
    printHelper(out, root);
}
template <typename T>
void BinaryTree<T>::printHelper(std::ostream &out, Node<T> *curr)
{
    if (!curr)
        return;
    out << curr->data << " ";
    printHelper(out, curr->left);
    printHelper(out, curr->right);
}
template <typename T>
Iterator<T> BinaryTree<T>::copy(Iterator<T> pos)
{
    if (pos)
    {
        Node<T> *left = pos.left() ? *copy(pos.left()).ptr : nullptr;
        Node<T> *right = pos.right() ? *copy(pos.right()).ptr : nullptr;
        root = new Node<T>(pos.get(), left, right);
    }
    return rootPos();
}
#endif