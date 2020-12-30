#include "graphicsscene.h"
#include "graphnode.h"

#include <QDebug>

GraphicsScene::GraphicsScene(QObject *parent)
{

}

GraphicsScene::~GraphicsScene()
{

}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // GraphNode* itemsList = (GraphNode*)itemAt(

    GraphNode* item = (GraphNode*)itemAt(event->scenePos(), QTransform());

    if(!item)
    {
        return;
    }

    if(event->modifiers() & Qt::ShiftModifier && event->button() == Qt::LeftButton)
    {
         item->setObstacle();
    }
    else if(event->button() == Qt::LeftButton)
    {
        if(start)
            start->resetNode();
        item->setStart();
        start = item;
    }
    else if(event->button() == Qt::RightButton)
    {
        if(end)
            end->resetNode();
        item->setEnd();
        end = item;
    }

    QGraphicsScene::mousePressEvent(event);
}

void GraphicsScene::setStartNode(GraphNode *node)
{
    if(start)
        start->resetNode();
    if(node)
        node->setParent(nullptr);

    start = node;
    start->setStart();
    qDebug() << "SN : " << start;
}

void GraphicsScene::setEndNode(GraphNode *node)
{
    if(end)
        end->resetNode();

    end = node;
    end->setEnd();
    qDebug() << "EN : " << end;
}

GraphNode* &GraphicsScene::getStartNode()
{
    return start;
}

GraphNode* &GraphicsScene::getEndNode()
{
    return end;
}
