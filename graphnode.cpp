#include "graphnode.h"
#include "mainwindow.h"
#include <QDebug>

GraphNode::GraphNode(int x, int y, qreal width, qreal height)
{
    this->x = x+1;
    this->y = y+1;

    rect = QRectF((y)*45, (x)*45, width, height);
    bPressed = false;
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
    QBrush brush(Qt::blue);

    //if(bPressed)
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
        hide();
    }
    else
    {
        brush.setColor(Qt::gray);
        setVisible(true);
    }

    if(bOpen && !bStart && !bEnd)
    {
        brush.setColor(Qt::cyan);
    }
    if(bClosed && !bStart && !bEnd)
    {
        brush.setColor(Qt::yellow);
    }
    if(bPath && !bStart && !bEnd)
    {
        brush.setColor(Qt::green);
    }


    painter->fillRect(rec, brush);
    painter->drawRect(rec);
    setFlag(ItemIsSelectable);
}

void GraphNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    bPressed = true;

    if(bStart)
    {
        MainWindow::setStartNode(nullptr);
        bStart = false;
    }
    if(bEnd)
    {
        MainWindow::setEndNode(nullptr);
        bEnd = false;
    }

    update();
    QGraphicsItem::mousePressEvent(event);
}

void GraphNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    bPressed = false;
//    bStart = false;
//    bEnd = false;
//    bObstacle = false;

    //update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void GraphNode::keyPressEvent(QKeyEvent *event)
{
    if(bPressed)
    {
        if(event->key() == Qt::Key_S)
        {
            MainWindow::setStartNode(this);
            bStart = true;
            gcost = 0;
        }
        else if(event->key() == Qt::Key_G)
        {
            MainWindow::setEndNode(this);
            bEnd = true;
            hcost = 0;
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

//void GraphNode::calculateGCost()
//{
//    gcost = parent->getGCost() + qSqrt((x - parent->getX())*(x - parent->getX()) + (y - parent->getY())*(y - parent->getY()));
//}

void GraphNode::setParent(GraphNode *node)
{
    parent = node;
}

void GraphNode::setVisited(bool bVal)
{
    bVisited = bVal;
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
    //_sleep(30);
    update();
}

void GraphNode::setPath()
{
    bPath = true;
    //_sleep(30);
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




