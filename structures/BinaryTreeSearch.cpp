#ifndef BIN_SEARCH_TREE
#define BIN_SEARCH_TREE

#include "BinaryTree.cpp"
template <typename T>
class BSTree : public BinaryTree<T>
{
    Iterator<T> search(T const &x)
    {
        Iterator pos = BinaryTree<T>::rootPos();
        while (pos && pos.get() != x)
        {
            if (x < pos.get())
                --pos;
            else
                ++pos;
        }
        return pos;
    }
    bool insert(T const &x)
    {
        Iterator<T> pos = search(x);
        if (pos)
            return false;
        BinaryTree<T> leaf = x;
        assignFrom(pos.left.rootPos());
    }
    bool remove(T const &x)
    {
        Iterator<T> pos = search(x);
        if (!pos)
            return false;
        if (!pos.left())
            assignFrom(pos, pos.right());
        if (!pos.right())
            assignFrom(pos, pos.left());
        Iterator<T> minRightTree = pos.right();
        while (minRightTree.left())
            --minRightTree;
        *pos = *minRightTree;
        assignFrom(minRightTree, minRightTree.right());
        return true;
    }
    void print(std::ostream *out)
    {
        BinaryTree<T>::printTree(out);
    }
};
#endif