#include "astar.h"
#include <QtCore>
#include <QtGui>

AStar::AStar()
{

}

// euclidian distance between nodes A and B
int AStar::distance(GraphNode* A, GraphNode* B)
{
    return qSqrt(100.f*((A->getX() - B->getX())*(A->getX() - B->getX()) + (A->getY() - B->getY())*(A->getY() - B->getY())));
}

bool AStar::isPresent(GraphNode* M, QQueue<GraphNode*> &list)
{
    for(auto& X : list)
    {
        if(X == M)
            return true;
    }
    return false;
}

void AStar::propagateImprovement(GraphNode* M, QQueue<GraphNode*> &closed, GraphNode* &endNode)
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

void AStar::addToOpen(GraphNode* M, QQueue<GraphNode*> &open)
{
    open.push_back(M);
    int n = open.size();

    // insertion sort - takes O(n) for sorted insertion
    for(int i = 1; i < n; i++)
    {
        int j = i - 1;
        auto x = open[i];
        while(j >= 0 && open[j]->getFCost() > x->getFCost())
        {
             open[j+1] = open[j];
             j--;
        }
        open[j+1] = x;
    }
}

void AStar::runAStar(GraphNode* &startNode, GraphNode* &endNode, QVector<QVector<GraphNode*>> &node, int& speed)
{
    int height = node.size();
    int width = node[0].size();

    // find neighbours of individual nodes
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            // ignoring obstacle nodes as neighbours
            if(node[i][j]->isObstacle())
                continue;

            // sides
            if(j > 0)
                node[i][j]->addNeighbour(node[i][j-1]);
            if(j < width - 1)
                node[i][j]->addNeighbour(node[i][j+1]);
            if(i > 0)
                node[i][j]->addNeighbour(node[i-1][j]);
            if(i < height - 1)
                node[i][j]->addNeighbour(node[i+1][j]);

            // diagonals
            if(i > 0 && j > 0)
                node[i][j]->addNeighbour(node[i-1][j-1]);
            if(i > 0 && j < width - 1)
                node[i][j]->addNeighbour(node[i-1][j+1]);
            if(j > 0 && i < height - 1)
                node[i][j]->addNeighbour(node[i+1][j-1]);
            if(i < height - 1 && j < width - 1)
                node[i][j]->addNeighbour(node[i+1][j+1]);
        }
    }

    //********************//
    // A* algorithm start //
    //********************//

    // always sorted with increasing f() values
    QQueue<GraphNode*> open;

    // list of nodes visited in order
    QQueue<GraphNode*> closed;

    // set f() and g() values of start node
    if(!startNode || !endNode)
    {
        qDebug() << "nullptr start/end nodes";
        return;
    }
    else
    {
        qDebug() << "AStar : EN : " << endNode;
    }
    startNode->setGCost(0);
    startNode->setFCost(startNode->getGCost() + startNode->getHCost(endNode));


    // only start node in open list
    open.push_back(startNode);

    while(!open.empty())
    {
        // pick a node with lowest f() value from open list
        auto& N = open.front();
        open.pop_front();

        // add that node to closed list
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

        // explore all neighbours of N
        for(auto& M : neighbours)
        {
            // found better path?
            if(N->getGCost() + distance(N , M) < M->getGCost())
            {
                M->setParent(N);
                M->setGCost(N->getGCost() + distance(N, M));
                M->setFCost(M->getGCost() + M->getHCost(endNode));

                // isPresentInOpen?
                if(isPresent(M, open))
                {
                    continue;
                }

                // isPresentInClosed?
                if(isPresent(M, closed))
                {
                    // update values of nodes in closed with new better(smaller) f() & g() values
                    propagateImprovement(M, closed, endNode);
                }
                else
                {
                    // add new node to open
                    addToOpen(M, open);
                    M->setOpen();
                }
            }
        }

        // update every 'x' milliseconds
        QCoreApplication::processEvents();
        Sleep(speed);
    }

    totalNodes = closed.size();
    qDebug() << "AStar : EN : " << endNode;
}

int AStar::getTotalNodes()
{
    return totalNodes;
}
