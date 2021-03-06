#ifndef BESTFIRSTSEARCH_H
#define BESTFIRSTSEARCH_H

#include "graphnode.h"


class BestFirstSearch
{
public:
    BestFirstSearch();

    bool isPresent(GraphNode *M, QList<GraphNode *> &list);

    void addToOpen(GraphNode *M, QList<GraphNode *> &open, GraphNode* endNode);

    void runBestFirstSearch(GraphNode* &startNode, GraphNode* &endNode, QVector<QVector<GraphNode*>> &node, int& speed);

    int getTotalNodes();

private:

    int totalNodes = 0;

};

#endif // BESTFIRSTSEARCH_H
