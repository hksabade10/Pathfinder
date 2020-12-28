#ifndef ASTAR_H
#define ASTAR_H

#include <graphnode.h>


class AStar
{
public:
    AStar();

    int distance(GraphNode* A, GraphNode* B);

    bool isPresent(GraphNode* M, QQueue<GraphNode*>& list);

    void propagateImprovement(GraphNode* M, QQueue<GraphNode*>& closed, GraphNode* &endNode);

    void addToOpen(GraphNode* M, QQueue<GraphNode*>& open);

    void runAStar(GraphNode* &startNode, GraphNode* &endNode, QVector<QVector<GraphNode*>> &node);


};

#endif // ASTAR_H
