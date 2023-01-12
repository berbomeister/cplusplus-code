#include <iostream>
#include "graph.cpp"
#include <vector>
#include <queue>
#include <stack>
/*template <typename T>
void DFSRec(Graph<T> &graph, T vertex, LinkedList<T> &visited, LinkedList<T> &result)
{
    visited.push(vertex);
    result.push(vertex);
    LinkedList<T> adj = graph.successors(vertex);
    for (int i = 0; i < adj.size(); i++)
    {
        if (!visited.find(adj.getAtIndex(i)))
            DFSRec(graph, adj.getAtIndex(i), visited, result);
    }
}
template <typename T>
void DFS(Graph<T> &graph, T start, LinkedList<T> &result)
{

    if (!graph.vertex(start))
        return;
    LinkedList<T> visited;
    DFSRec(graph, start, visited, result);
}
template <typename T>
void pathlist(Graph<T> &graph, T from, T to, LinkedList<T> &result)
{
    LinkedList<T> temp;
    DFS(graph, from, temp);
    if (!temp.find(to))
    {
        return;
    }
    Node<T> *p = temp.begin();
    while (p && p->data != to)
    {
        result.push(p->data);
        p = p->next;
    }
    result.push(p->data);
}*/
void BFS(Graph<int> &graph, int start, LinkedList<int> &result)
{
    if (!graph.vertex(start))
        return;
    std::vector<bool> visited(graph.vertices().size());
    std::queue<int> q;
    q.push(start);
    visited[start] = true;
    while (!q.empty())
    {
        int current = q.front();
        q.pop();
        result.push(current);
        LinkedList<int> adj = graph.successors(current);
        for (int i = 0; i < adj.size(); i++)
        {
            if (visited[adj.getAtIndex(i)])
                continue;
            visited[adj.getAtIndex(i)] = true;
            q.push(adj.getAtIndex(i));
        }
    }
}
void DFSRec(Graph<int> &graph, int vertex, std::vector<bool> &visited, LinkedList<int> &result)
{
    visited[vertex] = true;
    result.push(vertex);
    LinkedList<int> adj = graph.successors(vertex);
    for (int i = 0; i < adj.size(); i++)
    {
        if (!visited[adj.getAtIndex(i)])
            DFSRec(graph, adj.getAtIndex(i), visited, result);
    }
}
void DFS(Graph<int> &graph, int start, LinkedList<int> &result)
{

    if (!graph.vertex(start))
        return;
    std::vector<bool> visited(graph.vertices().size());
    DFSRec(graph, start, visited, result);
}
bool path(Graph<int> &graph, int from, int to)
{
    if (graph.empty() || !graph.vertex(from) || !graph.vertex(to))
        return false;
    if (from == to)
        return true;
    LinkedList<int> reach;
    DFS(graph, from, reach);
    return reach.find(to);
}
LinkedList<int> pathlist(Graph<int> &graph, int from, int to)
{
    LinkedList<int> reach;
    DFS(graph, from, reach);
    if (!reach.find(to))
        return LinkedList<int>();
    Node<int> *p = reach.begin();
    while (p && p->data != to)
    {
        p = p->next;
    }
    while (p->next)
    {
        Node<int> *temp = p->next;
        p->next = temp->next;
        delete temp;
    }
    return reach;
}
void allpaths(Graph<int> &graph, int from, int to, LinkedList<int> &list)
{
    list.push(from);
    if (from == to)
    {
        list.print();
    }
    else
    {
        LinkedList<int> succ = graph.successors(from);
        Node<int> *p = succ.begin();
        while (p)
        {
            if (!list.find(p->data))
            {
                allpaths(graph, p->data, to, list);
            }
            p = p->next;
        }
    }
    list.removeLast();
}
bool cycleRec(Graph<int> &graph, std::vector<bool> &visited, std::vector<bool> &stack, int currentVertex)
{
    visited[currentVertex] = true;
    stack[currentVertex] = true;

    LinkedList<int> succ = graph.successors(currentVertex);

    for (int i = 0; i < succ.size(); i++)
    {
        if (stack[succ.getAtIndex(i)])
        {
            return true;
        }

        if (!visited[succ.getAtIndex(i)])
        {
            bool result = cycleRec(graph, visited, stack, succ.getAtIndex(i));
            if (result)
                return true;
        }
    }
    stack[currentVertex] = false;
    return false;
}

bool cyclic(Graph<int> &graph)
{
    std::vector<bool> visited(graph.vertices().size());
    std::vector<bool> stack(graph.vertices().size());
    for (int i = 0; i < graph.vertices().size(); i++)
    {
        if (!visited[i])
        {
            bool result = cycleRec(graph, visited, stack, i);

            if (result)
                return true;
        }
    }
    return false;
}
void tsortRec(Graph<int> &graph, std::vector<bool> &visited, LinkedList<int> &result, int curr)
{
    visited[curr] = true;
    LinkedList<int> succ = graph.successors(curr);

    for (int i = 0; i < succ.size(); i++)
    {
        if (!visited[succ.getAtIndex(i)])
            tsortRec(graph, visited, result, succ.getAtIndex(i));
    }
    result.push(curr);
}
LinkedList<int> toposort(Graph<int> &graph)
{
    if (cyclic(graph))
        throw "Cyclic graph";
    std::vector<bool> visited(graph.vertices().size());
    LinkedList<int> result;
    for (int i = 1; i <= graph.vertices().size(); i++)
    {
        if (!visited[i])
            tsortRec(graph, visited, result, i);
    }
    result.reverse();
    return result;
}
int main()
{
    Graph<int> graph;
    for (int i = 1; i <= 10; i++)
        graph.addVertex(i);
    graph.addEdge(1, 3);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(2, 4);
    graph.addEdge(4, 3);
    graph.addEdge(4, 5);
    graph.addEdge(5, 6);
    graph.addEdge(5, 8);
    graph.addEdge(6, 7);
    graph.addEdge(7, 8);
    graph.addEdge(7, 9);
    graph.addEdge(10, 9);
    graph.addEdge(8, 10);
    //graph.addEdge(7, 2);
    //graph.print();
    LinkedList<int> bfsres, dfsres;
    BFS(graph, 1, bfsres);
    DFS(graph, 1, dfsres);
    //bfsres.print();
    //dfsres.print();
    //std::cout << std::boolalpha << cyclic(graph) << std::endl;
    LinkedList<int> list;
    //pathlist(graph, 5, 9).print();
    allpaths(graph, 5, 9, list);
    LinkedList<int> tsort = toposort(graph);
    //tsort.print();
    return 0;
}