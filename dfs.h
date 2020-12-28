#ifndef DFS_H
#define DFS_H

#include "graphnode.h"

class DFS
{
public:
    DFS();

    bool isPresent(GraphNode* M, QList<GraphNode*>& list);

    void runDFS(GraphNode* &startNode, GraphNode* &endNode, QVector<QVector<GraphNode*>> &node);
};

#endif // DFS_H
