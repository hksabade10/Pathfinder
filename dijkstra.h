#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <graphnode.h>


class Dijkstra
{
public:
    Dijkstra();

    int distance(GraphNode* A, GraphNode* B);

    bool isPresent(GraphNode* M, QQueue<GraphNode*>& list);

    void propagateImprovement(GraphNode* M, QQueue<GraphNode*>& closed, GraphNode* &endNode);

    void addToOpen(GraphNode* M, QQueue<GraphNode*>& open);

    void runDijkstra(GraphNode* &startNode, GraphNode* &endNode, QVector<QVector<GraphNode*>> &node);


};

#endif // DIJKSTRA_H
