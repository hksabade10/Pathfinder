#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include "graphnode.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static void setStartNode(GraphNode* node);
    static void setEndNode(GraphNode* node);

    int distance(GraphNode* A, GraphNode* B);

    bool isPresent(GraphNode* M, QQueue<GraphNode*>& list);

    void propagateImprovement(GraphNode* M, QQueue<GraphNode*>& closed);

    void addToOpen(GraphNode* M, QQueue<GraphNode*>& open);

    void reconstructPath();

private slots:
    void on_startAlgorithm_clicked();


private:
    Ui::MainWindow *ui;

    QGraphicsScene* scene;

    GraphNode* node[20][30];

    static GraphNode* startNode;
    static GraphNode* endNode;

};
#endif // MAINWINDOW_H
