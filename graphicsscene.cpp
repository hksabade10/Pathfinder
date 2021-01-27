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
    GraphNode* item = (GraphNode*)itemAt(event->scenePos(), QTransform());

    if(!item)
        return;

    if(event->button() == Qt::MiddleButton)
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

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        GraphNode* item = (GraphNode*)itemAt(event->scenePos(), QTransform());

        if(!item || item == start || item == end)
            return;

        if(event->modifiers() & Qt::ShiftModifier)
        {
            item->resetObstacle();
        }
        else
        {
            item->setObstacle();
        }
    }
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

