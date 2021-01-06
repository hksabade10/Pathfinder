#include "graphnode.h"
#include <QtMath>

GraphNode::GraphNode(int x, int y, qreal width, qreal height)
{
    this->x = x+1;
    this->y = y+1;

    rect = QRectF((y)*26, (x)*26, width, height);
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
    QBrush brush(Qt::white);

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
    else if(bInspecting)
    {
        brush.setColor(Qt::darkCyan);
    }
    else if(bOpen)
    {
        brush.setColor(Qt::cyan);
    }
    else
    {
        brush.setColor(Qt::white);
        // show();
    }

    painter->fillRect(rec, brush);
    painter->drawRect(rec);
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

void GraphNode::setInspecting()
{
    bInspecting = true;
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

void GraphNode::setObstacle()
{
    bObstacle = true;
    update();
}

void GraphNode::resetNode()
{
    bClosed = false;
    bInspecting = false;
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





