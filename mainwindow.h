#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>

#include "graphicsscene.h"
//#include "graphnode.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mousePressedEvent(QMouseEvent event);

    void resetScreen();

private slots:
    void on_startAlgorithm_clicked();

    void on_clearButton_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_speedSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    GraphicsScene* scene;

    QVector<QVector<GraphNode*>> node;

    int width = 30;
    int height = 20;

    int speed = 20;

};
#endif // MAINWINDOW_H
