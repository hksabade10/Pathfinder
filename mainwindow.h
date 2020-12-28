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

    void mousePressedEvent(QMouseEvent event);

    void resetScreen();

private slots:
    void on_startAlgorithm_clicked();

    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;

    QGraphicsScene* scene;

    QVector<QVector<GraphNode*>> node;

    static GraphNode* startNode;
    static GraphNode* endNode;

};
#endif // MAINWINDOW_H
