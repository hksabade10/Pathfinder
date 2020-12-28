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

void Dijkstra::runDijkstra(GraphNode *&startNode, GraphNode *&endNode, QVector<QVector<GraphNode *> > &node)
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

    //**************************//
    // Dijkstra algorithm start //
    //**************************//

    // always sorted with increasing f() values
    QQueue<GraphNode*> open;

    // list of nodes visited in order
    QQueue<GraphNode*> closed;

    // Debug
    if(!startNode || !endNode)
    {
        qDebug() << "nullptr start/end nodes";
        return;
    }
    else
    {
        qDebug() << "Diakstra : EN : " << endNode;
    }


    // g() value of start node
    startNode->setGCost(0);


    // only start node in open list
    open.push_back(startNode);

    while(!open.empty())
    {
        // pick a node with lowest g() value from open list
        auto& N = open.front();
        open.pop_front();

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

        QVector<GraphNode*> neighbours = N->getNeighbours();

        // explore all neighbours of N
        for(auto& M : neighbours)
        {
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

        // update every 'x' milliseconds
        QCoreApplication::processEvents();
        Sleep(20);
    }

    // Debug
    qDebug() << "Dijkstra : EN : " << endNode;


}
