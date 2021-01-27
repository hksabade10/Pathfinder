#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "astar.h"
#include "bestfirstsearch.h"
#include "bfs.h"
#include "dfs.h"
#include "dijkstra.h"
#include "mapdialog.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new GraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    ui->graphicsView->setMouseTracking(true);

    ui->algoComboBox->addItem("A*");
    ui->algoComboBox->addItem("Dijkstra");
    ui->algoComboBox->addItem("Best First Search");
    ui->algoComboBox->addItem("BFS");
    ui->algoComboBox->addItem("DFS");

    height = 35;
    width = 55;

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

    // generate neighbours
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(i > 0)
                node[i][j]->addNeighbour(node[i-1][j]);     // top
            if(j < width - 1)
                node[i][j]->addNeighbour(node[i][j+1]);     // right
            if(i < height - 1)
                node[i][j]->addNeighbour(node[i+1][j]);     // bottom
            if(j > 0)
                node[i][j]->addNeighbour(node[i][j-1]);     // left
            if(i > 0 && j < width - 1)
                node[i][j]->addNeighbour(node[i-1][j+1]);   // top-right
            if(i < height - 1 && j < width - 1)
                node[i][j]->addNeighbour(node[i+1][j+1]);   // bottom-right
            if(j > 0 && i < height - 1)
                node[i][j]->addNeighbour(node[i+1][j-1]);   // bottom-left
            if(i > 0 && j > 0)
                node[i][j]->addNeighbour(node[i-1][j-1]);   // top-left
        }
    }

    // default position for start and end nodes
    scene->setStartNode(node[2][2]);
    scene->setEndNode(node[height-3][width-3]);

    startup();

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

        // update every 'x' milliseconds
        QEventLoop loop;
        QTimer::singleShot(1, &loop, SLOT(quit()));
        loop.exec();
    }  

    scene->setStartNode(scene->getStartNode());
    scene->setEndNode(scene->getEndNode());
}


void MainWindow::read(const QJsonObject &json)
{
    QString mName = ui->mapComboBox->currentText();

    if(json.contains(mName) && json[mName].isArray())
    {
        QJsonArray jArray = json[mName].toArray();

        for(auto arr : jArray)
        {
            if(arr.isArray())
            {
                QJsonArray jArr = arr.toArray();
                node[jArr[0].toInt()][jArr[1].toInt()]->setObstacle();
            }
        }
    }
}

void MainWindow::write(QJsonObject &json, QString mName) const
{
    QJsonArray jArray;
    for(int i = 0; i < node.size(); i++)
    {
        for(int j = 0; j < node[0].size(); j++)
        {
            QJsonArray jArr;
            if(node[i][j]->isObstacle())
            {
                jArr.append(i);
                jArr.append(j);
                jArray.append(jArr);
            }
        }
    }

    json.insert(mName, jArray);
}

void MainWindow::startup()
{
    QFile loadFile(QStringLiteral("save.json"));

    if (!loadFile.open(QIODevice::ReadOnly)) {
       qWarning("Couldn't open save file.");
       return;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    mObj = loadDoc.object();
    ui->mapComboBox->addItems(mObj.keys());
}


void MainWindow::on_startAlgorithm_clicked()
{

    ui->startAlgorithm->setEnabled(false);

    if(!scene->getStartNode() || !scene->getEndNode())
    {
        qDebug() << "nullptr start/end nodes";
        return;
    }

    // select algorithm to run from comboBox
    QString selection = ui->algoComboBox->currentText();

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

        // update every 'x' milliseconds
        QEventLoop loop;
        QTimer::singleShot(speed, &loop, SLOT(quit()));
        loop.exec();
    }

    ui->startAlgorithm->setEnabled(true);
}

void MainWindow::on_clearButton_clicked()
{
    resetScreen();
    ui->costBox->clear();
    ui->nodesBox->clear();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    ui->speedSlider->setValue(arg1);
}

void MainWindow::on_speedSlider_valueChanged(int value)
{
    speed = value;
    ui->spinBox->setValue(value);
}

void MainWindow::on_saveButton_clicked()
{
    MapDialog map;

    map.setModal(true);
    map.exec();

    mName = map.getName();

    if(mName == "")
        return;

    if(!mObj.contains(mName))
    {
        ui->mapComboBox->addItem(mName);
    }
    else
    {
        QMessageLogger msg;
        msg.warning("duplicate overwritten!");
    }

    QFile saveFile(QStringLiteral("save.json"));

    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save file.");
        return;
    }

    write(mObj, mName);
    saveFile.write(QJsonDocument(mObj).toJson(QJsonDocument::Compact));

    qDebug() << "saving...";

}

void MainWindow::on_generateButton_clicked()
{
    resetScreen();
    read(mObj);
}

void MainWindow::on_deleteButton_clicked()
{

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete?", "Are you sure you want to delete " + ui->mapComboBox->currentText() + "?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes && ui->mapComboBox->currentText() != QString("Default"))
    {
        mObj.remove(ui->mapComboBox->currentText());
        ui->mapComboBox->removeItem(ui->mapComboBox->currentIndex());

        QFile saveFile(QStringLiteral("save.json"));

        if (!saveFile.open(QIODevice::WriteOnly))
        {
            qWarning("Couldn't open save file.");
            return;
        }
        saveFile.write(QJsonDocument(mObj).toJson(QJsonDocument::Compact));
    }
}
