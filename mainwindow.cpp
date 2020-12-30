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

    scene = new GraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    ui->comboBox->addItem("A*");
    ui->comboBox->addItem("Dijkstra");
    ui->comboBox->addItem("Best First Search");
    ui->comboBox->addItem("BFS");
    ui->comboBox->addItem("DFS");



    height = 35;
    width = 50;

    node.resize(height);
    // add nodes in the window
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            node[i].push_back(new GraphNode(i, j, 22, 22));
            scene->addItem(node[i][j]);
        }
    }

    // default position for start and end nodes
    scene->setStartNode(node[9][9]);
    scene->setEndNode(node[9][40]);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resetScreen()
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(node[i][j] == scene->getStartNode() || node[i][j] == scene->getEndNode())
                continue;

            node[i][j]->resetNode();
        }

        QCoreApplication::processEvents();
        Sleep(1);
    }  

    scene->setStartNode(scene->getStartNode());
    scene->setEndNode(scene->getEndNode());
}

void MainWindow::on_startAlgorithm_clicked()
{


    if(!scene->getStartNode() || !scene->getEndNode())
    {
//        QMessageLogger message()
//        message.info();
        qDebug() << "nullptr start/end nodes";
        return;
    }

    // select algorithm to run from comboBox
    QString selection = ui->comboBox->currentText();

    if(selection == QString("A*"))
    {
        AStar algo;
        algo.runAStar(scene->getStartNode(), scene->getEndNode(), node, speed);
        ui->nodesBox->setText(QString::number(algo.getTotalNodes()));
    }
    else if(selection == QString("Dijkstra"))
    {
        Dijkstra algo;
        algo.runDijkstra(scene->getStartNode(), scene->getEndNode(), node, speed);
        ui->nodesBox->setText(QString::number(algo.getTotalNodes()));
    }
    else if(selection == "Best First Search")
    {
        BestFirstSearch algo;
        algo.runBestFirstSearch(scene->getStartNode(), scene->getEndNode(), node, speed);
        ui->nodesBox->setText(QString::number(algo.getTotalNodes()));
    }
    else if(selection == QString("BFS"))
    {
        BFS algo;
        algo.runBFS(scene->getStartNode(), scene->getEndNode(), node, speed);
        ui->nodesBox->setText(QString::number(algo.getTotalNodes()));
    }
    else if(selection == QString("DFS"))
    {
        DFS algo;
        algo.runDFS(scene->getStartNode(), scene->getEndNode(), node, speed);
        ui->nodesBox->setText(QString::number(algo.getTotalNodes()));
    }


    // display cost on ui
    if(scene->getEndNode()->getGCost() == INT32_MAX)
        ui->costBox->setText(QString("NA"));
    else
        ui->costBox->setText(QString::number(scene->getEndNode()->getGCost()));


    // reconstruct path from end to start
    auto ptr = scene->getEndNode();
    while(ptr)
    {
        ptr->setPath();
        ptr = ptr->getParent();
        QCoreApplication::processEvents();
        Sleep(5);
    }
}

void MainWindow::on_clearButton_clicked()
{
    resetScreen();
    ui->costBox->clear();
    ui->nodesBox->clear();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    speed = arg1;
    ui->speedSlider->setValue(speed);
}

void MainWindow::on_speedSlider_valueChanged(int value)
{
    ui->spinBox->setValue(value);
}
