#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    QBrush blueBrush(Qt::blue);
    QBrush grayBrush(Qt::gray);
    QPen blackPen(Qt::black);
    blackPen.setWidth(1);

    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 30; j++)
        {
            node[i][j] = new GraphNode(i, j, 40, 40);
            scene->addItem(node[i][j]);
        }
    }




//    QGraphicsItem* node = new QGraphicsRectItem(0, 0, 40, 40);
//    node = scene->addRect(0, 0, 40, 40, blackPen, grayBrush);
//    scene->addItem(node);
//    node->setFlag(QGraphicsItem::ItemIsSelectable);
//    scene->addRect((j+1)*45, (i+1)*45, 40, 40, blackPen, grayBrush);
}

MainWindow::~MainWindow()
{
    delete ui;
}

GraphNode* MainWindow::startNode = nullptr;
GraphNode* MainWindow::endNode = nullptr;

void MainWindow::setStartNode(GraphNode* node)
{
    startNode = node;
    if(node)
        node->setParent(nullptr);

    node->getGCost();

    qDebug() << "SN : " << startNode;
}
void MainWindow::setEndNode(GraphNode* node)
{
    endNode = node;
    qDebug() << "EN : " << endNode;
}

int MainWindow::distance(GraphNode *A, GraphNode *B)
{
    return qSqrt(100.f*((A->getX() - B->getX())*(A->getX() - B->getX()) + (A->getY() - B->getY())*(A->getY() - B->getY())));
}

bool MainWindow::isPresent(GraphNode *M, QQueue<GraphNode *>& list)
{
    for(auto& X : list)
    {
        if(X == M)
            return true;
    }
    return false;
}

void MainWindow::propagateImprovement(GraphNode *M, QQueue<GraphNode*>& closed)
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
                propagateImprovement(X, closed);
         }
    }
}

void MainWindow::addToOpen(GraphNode *M, QQueue<GraphNode *> &open)
{
    open.push_back(M);
    int n = open.size();


    // insertion sort
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

void MainWindow::reconstructPath()
{
    auto ptr = endNode;
    while(ptr)
    {
        ptr->setPath();
        ptr = ptr->getParent();
        QCoreApplication::processEvents();
        Sleep(10);
    }
}

void MainWindow::on_startAlgorithm_clicked()
{
    // find individual neighbours
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 30; j++)
        {

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


    // reset
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 30; j++)
        {
            if(node[i][j] == startNode || node[i][j] == endNode)
                continue;

            node[i][j]->setVisited(false);
            node[i][j]->setGCost(INT32_MAX);
            node[i][j]->setHCost(INT32_MAX);
            node[i][j]->setFCost(INT32_MAX);
            //node[i][j]->setParent(nullptr);
        }
    }

    // algo start
    QQueue<GraphNode*> open;
    QQueue<GraphNode*> closed;

    startNode->setGCost(0);
    startNode->setFCost(startNode->getGCost() + startNode->getHCost(endNode));
    open.push_back(startNode);

    while(!open.empty())
    {
        auto& N = open.front();
        open.pop_front();

        closed.push_back(N);
        N->setClosed();



        if(N == endNode)
        {
            reconstructPath();
            break;
        }

        QVector<GraphNode*> neighbours = N->getNeighbours();

        // set parent (M, N)
        for(auto& M : neighbours)
        {
            if(N->getGCost() + distance(N , M) < M->getGCost())
            {
                M->setParent(N);
                M->setGCost(N->getGCost() + distance(N , M));
                M->setFCost(M->getGCost() + M->getHCost(endNode));

                if(isPresent(M, open))  //isPresentInOpen
                {
                    continue;
                }
                if(isPresent(M, closed))    //isPresentInClosed
                {
                    propagateImprovement(M, closed);
                }
                else
                {
                    addToOpen(M, open);
                    M->setOpen();
                }
            }
        }

        // update every x milliseconds
        QCoreApplication::processEvents();
        Sleep(30);
    }
}
