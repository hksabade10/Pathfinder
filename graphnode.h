#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <QGraphicsItem>
#include <QPainter>


class GraphNode : public QGraphicsItem
{
public:
    GraphNode(int x, int y, qreal width, qreal height);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;


    void addNeighbour(GraphNode* node);


    // setters
    void setParent(GraphNode* node);

    void setGCost(int val);

    void setHCost(int val);

    void setFCost(int val);

    void setClosed();

    void setInspecting();

    void setOpen();

    void setPath();

    void setStart();

    void setEnd();

    void setObstacle();

    void resetObstacle();

    void resetNode();



    // getters
    int getX();

    int getY();

    int getFCost();

    int getGCost();

    int getHCost(GraphNode* endNode);

    GraphNode* getParent();

    QVector<GraphNode*> getNeighbours();

    bool isObstacle();


private:

    QRectF rect;

    int gcost = INT32_MAX;
    int hcost = INT32_MAX;
    int fcost = INT32_MAX;

    bool bObstacle = false;
    bool bClosed = false;
    bool bInspecting = false;
    bool bOpen = false;
    bool bPath = false;

    int x;
    int y;

    bool bStart = false;
    bool bEnd = false;

    QVector<GraphNode*> neighbours;

    GraphNode* parent;
};


#endif // GRAPHNODE_H
