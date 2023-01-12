#ifndef GRAPH
#define GRAPH

#include <iostream>
#include "linkedlist.cpp"
template <typename T>
struct TreeNode
{
    T data;
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;
    TreeNode(T data) : data(data) {}
};
template <typename T>
class Graph
{
    LinkedList<LinkedList<T>> graph;
    LinkedList<T> *findVertexList(T const &x)
    {
        Node<LinkedList<T>> *p = graph.begin();
        Node<T> *temp;
        while (p)
        {
            temp = p->data.begin();
            if (temp->data == x)
                return &(p->data);
            p = p->next;
        }
        return nullptr;
    }

public:
    void addVertex(T const &x);                  //
    void deleteVertex(T const &x);               //
    void addEdge(T const &from, T const &to);    //
    void deleteEdge(T const &from, T const &to); //
    bool vertex(T const &x);                     //
    bool edge(T const &from, T const &to);       //
    bool empty();                                //
    Node<T> *point(T const &x);                  //
    LinkedList<T> vertices();                    //
    LinkedList<T> successors(T const &x);        //
    void print();
};
template <typename T>
void Graph<T>::addVertex(T const &x)
{
    LinkedList<T> vertexList;
    vertexList.push(x);
    graph.push(vertexList);
}
template <typename T>
void Graph<T>::addEdge(T const &from, T const &to)
{
    if (empty() || !vertex(from) || !vertex(to))
        return;
    Node<LinkedList<T>> *p = graph.begin();
    Node<T> *temp;
    temp = p->data.begin();
    while (temp->data != from)
    {
        p = p->next;
        temp = p->data.begin();
    }
    p->data.push(to);
}
template <typename T>
void Graph<T>::deleteVertex(T const &x)
{
    if (!vertex(x))
        return;
    Node<LinkedList<T>> *p = graph.begin();
    while (p)
    {
        Node<T> *q = p->data.begin();
        if (edge(q->data, x))
            deleteEdge(q->data, x);
        p = p->next;
    }
    Node<LinkedList<T>> *r = graph.begin();
    Node<T> *temp;
    do
    {
        temp = r->data.begin();
        if (temp->data == x)
            break;
        r = r->next;
    } while (r);
    graph.removeNode(r);
}
template <typename T>
void Graph<T>::deleteEdge(T const &from, T const &to)
{
    if (empty() || !edge(from, to))
        return;
    Node<LinkedList<T>> *p = graph.begin();
    Node<T> *q = p->data.begin();
    while (q->data != from)
    {
        p = p->next;
        q = p->data.begin();
    }
    q = q->next;
    while (q->data != to)
        q = q->next;
    p->data.removeNode(q);
}
template <typename T>
bool Graph<T>::vertex(T const &x)
{
    return findVertexList(x) != nullptr;
}
template <typename T>
bool Graph<T>::edge(T const &from, T const &to)
{
    if (empty())
        return false;
    LinkedList<T> l = *findVertexList(from);
    if (l.empty())
        return false;
    Node<T> *p = l.begin();
    p = p->next;
    while (p)
    {
        if (p->data == to)
            return true;
        p = p->next;
    }
    return false;
}
template <typename T>
bool Graph<T>::empty()
{
    return graph.empty();
}
template <typename T>
LinkedList<T> Graph<T>::vertices()
{
    LinkedList<T> result;
    Node<LinkedList<T>> *p = graph.begin();
    Node<T> *temp;
    while (p)
    {
        temp = p->data.begin();
        result.push(temp->data);
        p = p->next;
    }
    return result;
}
template <typename T>
LinkedList<T> Graph<T>::successors(T const &x)
{
    if (!vertex(x))
        return LinkedList<T>();
    LinkedList<T> l = *findVertexList(x);
    l.removeFirst();
    return l;
}
template <typename T>
void Graph<T>::print()
{
    Node<LinkedList<T>> *p = graph.begin();
    while (p)
    {
        p->data.print();
        p = p->next;
    }
    //std::cout << std::endl;
}
#endif