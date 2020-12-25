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

    void setVisited(bool val);

    void setGCost(int val);

    void setHCost(int val);

    void setFCost(int val);

    void setClosed();

    void setOpen();

    void setPath();

    // getters
    int getX();

    int getY();

    int getFCost();

    int getGCost();

    int getHCost(GraphNode* endNode);

    GraphNode *getParent();

    QVector<GraphNode *> getNeighbours();



    bool isObstacle();

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;


private:

    QRectF rect;
    bool bPressed;

    int gcost = INT32_MAX;
    int hcost = INT32_MAX;
    int fcost = INT32_MAX;

    bool bVisited = false;
    bool bObstacle = false;
    bool bClosed = false;
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
