#include "bfs.h"
#include <QtCore>
#include <QtGui>

BFS::BFS()
{

}

bool BFS::isPresent(GraphNode *M, QList<GraphNode *> &list)
{
    for(auto& X : list)
    {
        if(X == M)
            return true;
    }
    return false;
}

void BFS::runBFS(GraphNode *&startNode, GraphNode *&endNode, QVector<QVector<GraphNode *> > &node, int& speed)
{

    //*********************//
    // BFS algorithm start //
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

            // isPresentInOpen/Closed?
            if(isPresent(M, open) || isPresent(M, closed))
            {
                continue;
            }
            else
            {
                unvisited.push_back(M);
                M->setParent(N);
                M->setOpen();
            }
        }
        open = open + unvisited;
    }

    totalNodes = closed.size();
}

int BFS::getTotalNodes()
{
    return totalNodes;
}
