#include "dijkstra.h"
#include <QtCore>
#include <QtGui>

Dijkstra::Dijkstra()
{

}

int Dijkstra::distance(GraphNode *A, GraphNode *B)
{
    return qSqrt(100.f*((A->getX() - B->getX())*(A->getX() - B->getX()) + (A->getY() - B->getY())*(A->getY() - B->getY())));
}

bool Dijkstra::isPresent(GraphNode *M, QQueue<GraphNode *> &list)
{
    for(auto& X : list)
    {
        if(X == M)
            return true;
    }
    return false;
}

void Dijkstra::propagateImprovement(GraphNode *M, QQueue<GraphNode *> &closed, GraphNode *&endNode)
{
    auto neighbours = M->getNeighbours();
    for(auto& X : neighbours)
    {
         if(M->getGCost() + distance(X, M) < X->getGCost())
         {
             X->setParent(M);
             X->setGCost(M->getGCost() + distance(X, M));
             X->setFCost(X->getGCost() + X->getHCost(endNode));
             if(isPresent(X, closed))
                propagateImprovement(X, closed, endNode);
         }
    }
}

void Dijkstra::addToOpen(GraphNode *M, QQueue<GraphNode *> &open)
{
    open.push_back(M);
    int n = open.size();

    // insertion sort - takes O(n) for sorted insertion
    for(int i = 1; i < n; i++)
    {
        int j = i - 1;
        auto x = open[i];
        while(j >= 0 && open[j]->getGCost() > x->getGCost())
        {
             open[j+1] = open[j];
             j--;
        }
        open[j+1] = x;
    }
}

void Dijkstra::runDijkstra(GraphNode *&startNode, GraphNode *&endNode, QVector<QVector<GraphNode *> > &node, int& speed)
{

    //**************************//
    // Dijkstra algorithm start //
    //**************************//

    // always sorted with increasing f() values
    QQueue<GraphNode*> open;

    // list of nodes visited in order
    QQueue<GraphNode*> closed;

    // g() value of start node
    startNode->setGCost(0);


    // only start node in open list
    open.push_back(startNode);

    while(!open.empty())
    {
        // pick a node with lowest g() value from open list
        auto& N = open.front();
        open.pop_front();
        N->setInspecting();

        // update every 'x' milliseconds
        QEventLoop loop;
        QTimer::singleShot(speed, &loop, SLOT(quit()));
        loop.exec();

        // goal node found?
        if(N == endNode)
        {
            // reconstructPath(endNode);
            qDebug() << "Goal Found";
            break;
        }

        // add visited node to closed list
        closed.push_back(N);
        N->setClosed();

        // explore all neighbours of N
        for(auto& M : N->getNeighbours())
        {
            if(M->isObstacle())
                continue;

            // found better path?
            if(N->getGCost() + distance(N , M) < M->getGCost())
            {
                M->setParent(N);
                M->setGCost(N->getGCost() + distance(N, M));

                // isPresentInOpen?
                if(isPresent(M, open))
                {
                    continue;
                }

                // isPresentInClosed?
                if(isPresent(M, closed))
                {
                    // update values of nodes in closed with new better(smaller) g() values
                    propagateImprovement(M, closed, endNode);
                }
                else
                {
                    // add new node to open(sorted insertion)
                    addToOpen(M, open);
                    M->setOpen();
                }
            }
        }
    }
    totalNodes = closed.size();
}

int Dijkstra::getTotalNodes()
{
    return totalNodes;
}
