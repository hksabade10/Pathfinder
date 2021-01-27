#include "bestfirstsearch.h"

#include <QtCore>
#include <QtGui>

BestFirstSearch::BestFirstSearch()
{

}

void BestFirstSearch::addToOpen(GraphNode* M, QList<GraphNode*> &open, GraphNode* endNode)
{
    open.push_back(M);
    int n = open.size();

    // insertion sort - takes O(n) for sorted insertion
    for(int i = 1; i < n; i++)
    {
        int j = i - 1;
        auto x = open[i];
        while(j >= 0 && open[j]->getHCost(endNode) > x->getHCost(endNode))
        {
             open[j+1] = open[j];
             j--;
        }
        open[j+1] = x;
    }
}


bool BestFirstSearch::isPresent(GraphNode *M, QList<GraphNode *> &list)
{
    for(auto& X : list)
    {
        if(X == M)
            return true;
    }
    return false;
}

void BestFirstSearch::runBestFirstSearch(GraphNode* &startNode, GraphNode* &endNode, QVector<QVector<GraphNode*>> &node, int& speed)
{

    //*********************//
    // DFS algorithm start //
    //*********************//

    // list of nodes to be visited
    QList<GraphNode*> open;

    // list of nodes visited
    QList<GraphNode*> closed;

    // only start node in open list
    open.push_back(startNode);

    while(!open.empty())
    {
        // pick last(top of stack) node from open list
        auto& N = open.front();
        open.pop_front();
        N->setInspecting();

        // update every 'x' milliseconds
        QEventLoop loop;
        QTimer::singleShot(speed, &loop, SLOT(quit()));
        loop.exec();

        // add visited node to closed list
        closed.push_back(N);
        N->setClosed();

        // goal node found?
        if(N == endNode)
        {
            // reconstructPath(endNode);
            qDebug() << "Goal Found";
            break;
        }

        QList<GraphNode*> unvisited;

        // explore all neighbours of N
        for(auto& M : N->getNeighbours())
        {
            if(M->isObstacle())
                continue;

            // isPresentInOpen?
            if(isPresent(M, open) || isPresent(M, closed))
            {
                continue;
            }
            else
            {
                unvisited.push_back(M);
                M->setParent(N);
                M->setOpen();

                addToOpen(M, open, endNode);
            }
        }
    }

    totalNodes = closed.size();
}

int BestFirstSearch::getTotalNodes()
{
    return totalNodes;
}
