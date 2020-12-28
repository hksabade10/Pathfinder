#ifndef BFS_H
#define BFS_H

#include "graphnode.h"


class BFS
{
public:
    BFS();

    bool isPresent(GraphNode* M, QList<GraphNode*>& list);

    void runBFS(GraphNode* &startNode, GraphNode* &endNode, QVector<QVector<GraphNode*>> &node);

private:

    int cost = 0;

};

#endif // BFS_H
