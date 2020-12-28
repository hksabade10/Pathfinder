#include "graphnode.h"
#include "mainwindow.h"
#include <QDebug>

GraphNode::GraphNode(int x, int y, qreal width, qreal height)
{
    this->x = x+1;
    this->y = y+1;

    rect = QRectF((y)*45, (x)*45, width, height);
    setFlag(ItemIsSelectable);
    setFlag(ItemIsFocusable);

}

QRectF GraphNode::boundingRect() const
{
    return rect;
}

void GraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::gray);


    if(bStart)
    {
        brush.setColor(Qt::red);
    }
    else if(bEnd)
    {
        brush.setColor(Qt::blue);
    }
    else if(bObstacle)
    {
        brush.setColor(Qt::black);
        // hide();
    }
    else if(bPath)
    {
        brush.setColor(Qt::green);
    }
    else if(bClosed)
    {
        brush.setColor(Qt::yellow);
    }
    else if(bOpen)
    {
        brush.setColor(Qt::cyan);
    }
    else
    {
        brush.setColor(Qt::gray);
        // show();
    }

    painter->fillRect(rec, brush);
    painter->drawRect(rec);
}

void GraphNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{    
    bPressed = true;

//    if(bStart)
//    {
//        MainWindow::setStartNode(nullptr);
//        bStart = false;
//    }
//    if(bEnd)
//    {
//        MainWindow::setEndNode(nullptr);
//        bEnd = false;
//    }

    update();
    QGraphicsItem::mousePressEvent(event);
}

void GraphNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    bPressed = false;

    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void GraphNode::keyPressEvent(QKeyEvent *event)
{
    if(bPressed)
    {
        if(event->key() == Qt::Key_S)
        {
            MainWindow::setStartNode(this);
            setStart();
        }
        else if(event->key() == Qt::Key_G)
        {
            MainWindow::setEndNode(this);
            setEnd();
        }
        else if(event->key() == Qt::Key_B)
        {
            bObstacle = true;
        }
        update();
    }
    QGraphicsItem::keyPressEvent(event);
}


void GraphNode::addNeighbour(GraphNode* node)
{
    neighbours.push_back(node);
}

void GraphNode::setParent(GraphNode *node)
{
    parent = node;
}

void GraphNode::setGCost(int val)
{
    gcost = val;
}

void GraphNode::setHCost(int val)
{
    hcost = val;
}

void GraphNode::setFCost(int val)
{
    fcost = val;
}

void GraphNode::setClosed()
{
    bClosed = true;
    update();
}

void GraphNode::setOpen()
{
    bOpen = true;
    update();
}

void GraphNode::setPath()
{
    bPath = true;
    update();
}

void GraphNode::setStart()
{
    gcost = 0;
    bStart = true;
    update();
}

void GraphNode::setEnd()
{
    hcost = 0;
    bEnd = true;
    update();
}

void GraphNode::resetNode()
{
    bClosed = false;
    bOpen = false;
    bPath = false;
    bObstacle = false;
    bStart = false;
    bEnd = false;

    gcost = INT32_MAX;
    hcost = INT32_MAX;
    fcost = INT32_MAX;

    parent = nullptr;

    neighbours.clear();

    update();
}

int GraphNode::getX()
{
    return x;
}

int GraphNode::getY()
{
    return y;
}

int GraphNode::getFCost()
{
    return fcost;
}

int GraphNode::getGCost()
{
    return gcost;
}

int GraphNode::getHCost(GraphNode* endNode)
{
    hcost = qSqrt(100.f*((x - endNode->getX())*(x - endNode->getX()) + (y - endNode->getY())*(y - endNode->getY())));
    return hcost;
}

GraphNode *GraphNode::getParent()
{
    return parent;
}

QVector<GraphNode *> GraphNode::getNeighbours()
{
    return neighbours;
}

bool GraphNode::isObstacle()
{
    return bObstacle;
}




