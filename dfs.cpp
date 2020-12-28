#include "dfs.h"
#include <QtCore>
#include <QtGui>


DFS::DFS()
{

}

bool DFS::isPresent(GraphNode *M, QList<GraphNode *> &list)
{
    for(auto& X : list)
    {
        if(X == M)
            return true;
    }
    return false;
}

void DFS::runDFS(GraphNode *&startNode, GraphNode *&endNode, QVector<QVector<GraphNode *>> &node)
{
    // find neighbours of individual nodes
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 30; j++)
        {
            // not adding obstacle nodes as neighbours
            if(node[i][j]->isObstacle())
                continue;


            if(i > 0)
                node[i][j]->addNeighbour(node[i-1][j]);     // top
            if(i > 0 && j < 30 - 1)
                node[i][j]->addNeighbour(node[i-1][j+1]);   // top-right
            if(j < 30 - 1)
                node[i][j]->addNeighbour(node[i][j+1]);     // right
            if(i < 20 - 1 && j < 30 - 1)
                node[i][j]->addNeighbour(node[i+1][j+1]);   // bottom-right
            if(i < 20 - 1)
                node[i][j]->addNeighbour(node[i+1][j]);     // bottom
            if(j > 0 && i < 20 - 1)
                node[i][j]->addNeighbour(node[i+1][j-1]);   // bottom-left
            if(j > 0)
                node[i][j]->addNeighbour(node[i][j-1]);     // left
            if(i > 0 && j > 0)
                node[i][j]->addNeighbour(node[i-1][j-1]);   // top-left

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
        qDebug() << "DFS : EN : " << endNode;
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
            }
        }

        open = unvisited + open;

        //M->setParent(N);

        // update every 'x' milliseconds
        QCoreApplication::processEvents();
        Sleep(20);
    }
}
