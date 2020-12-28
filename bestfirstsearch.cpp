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

void BestFirstSearch::runBestFirstSearch(GraphNode* &startNode, GraphNode* &endNode, QVector<QVector<GraphNode*>> &node)
{
    // find neighbours of individual nodes
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 30; j++)
        {
            // not adding obstacle nodes as neighbours
            if(node[i][j]->isObstacle())
                continue;

            // sides
            if(j > 0)
                node[i][j]->addNeighbour(node[i][j-1]);
            if(j < 30 - 1)
                node[i][j]->addNeighbour(node[i][j+1]);
            if(i > 0)
                node[i][j]->addNeighbour(node[i-1][j]);
            if(i < 20 - 1)
                node[i][j]->addNeighbour(node[i+1][j]);

            // diagonals
            if(i > 0 && j > 0)
                node[i][j]->addNeighbour(node[i-1][j-1]);
            if(i > 0 && j < 30 - 1)
                node[i][j]->addNeighbour(node[i-1][j+1]);
            if(j > 0 && i < 20 - 1)
                node[i][j]->addNeighbour(node[i+1][j-1]);
            if(i < 20 - 1 && j < 30 - 1)
                node[i][j]->addNeighbour(node[i+1][j+1]);
        }
    }

    //*********************//
    // DFS algorithm start //
    //*********************//

    // list of nodes to be visited
    QList<GraphNode*> open;

    // list of nodes visited
    QList<GraphNode*> closed;

    // Debug
    if(!startNode || !endNode)
    {
        qDebug() << "nullptr start/end nodes";
        return;
    }
    else
    {
        qDebug() << "BestFirstSearch : EN : " << endNode;
    }

    // only start node in open list
    open.push_back(startNode);

    while(!open.empty())
    {
        // pick last(top of stack) node from open list
        auto& N = open.front();
        open.pop_front();

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

        QVector<GraphNode*> neighbours = N->getNeighbours();
        QList<GraphNode*> unvisited;

        // explore all neighbours of N
        for(auto& M : neighbours)
        {
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

        // update every 'x' milliseconds
        QCoreApplication::processEvents();
        Sleep(20);
    }
}