#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "astar.h"
#include "bestfirstsearch.h"
#include "bfs.h"
#include "dfs.h"
#include "dijkstra.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    ui->comboBox->addItem("A*");
    ui->comboBox->addItem("Dijkstra");
    ui->comboBox->addItem("Best First Search");
    ui->comboBox->addItem("BFS");
    ui->comboBox->addItem("DFS");


    node.resize(20);

    // add nodes in the window
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 30; j++)
        {
            node[i].push_back(new GraphNode(i, j, 40, 40));
            scene->addItem(node[i][j]);
        }
    }

    setStartNode(node[4][9]);
    setEndNode(node[15][20]);
}

MainWindow::~MainWindow()
{
    delete ui;
}


GraphNode* MainWindow::startNode = nullptr;
GraphNode* MainWindow::endNode = nullptr;


void MainWindow::setStartNode(GraphNode* node)
{
    if(startNode)
        startNode->resetNode();
    if(node)
        node->setParent(nullptr);

    startNode = node;
    startNode->setStart();
    qDebug() << "SN : " << startNode;
}

void MainWindow::setEndNode(GraphNode* node)
{
    if(endNode)
        endNode->resetNode();

    endNode = node;
    endNode->setEnd();
    qDebug() << "EN : " << endNode;
}


void MainWindow::resetScreen()
{
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 30; j++)
        {
            if(node[i][j] == startNode || node[i][j] == endNode)
                continue;

            node[i][j]->resetNode();
        }

        QCoreApplication::processEvents();
        Sleep(2);
    }

    setStartNode(startNode);
    setEndNode(endNode);
}

void MainWindow::on_startAlgorithm_clicked()
{
    QString selection = ui->comboBox->currentText();

    if(selection == QString("A*"))
    {
        AStar algo;
        algo.runAStar(startNode, endNode, node);
    }
    else if(selection == QString("Dijkstra"))
    {
        Dijkstra algo;
        algo.runDijkstra(startNode, endNode, node);
    }
    else if(selection == "Best First Search")
    {
        BestFirstSearch algo;
        algo.runBestFirstSearch(startNode, endNode, node);
    }
    else if(selection == QString("BFS"))
    {
        BFS algo;
        algo.runBFS(startNode, endNode, node);
    }
    else if(selection == QString("DFS"))
    {
        DFS algo;
        algo.runDFS(startNode, endNode, node);
    }


    if(endNode->getGCost() == INT32_MAX)
        ui->costBox->setText(QString("NA"));
    else
        ui->costBox->setText(QString::number(endNode->getGCost()));



    // reconstruct path
    auto ptr = endNode;

    while(ptr)
    {
        ptr->setPath();
        ptr = ptr->getParent();
        QCoreApplication::processEvents();
        Sleep(10);
    }

}

void MainWindow::on_clearButton_clicked()
{
    resetScreen();
    ui->costBox->clear();
}
