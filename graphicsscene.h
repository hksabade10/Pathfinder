#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

class GraphNode;


class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = 0);

    virtual ~GraphicsScene();

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void setStartNode(GraphNode* node);

    void setEndNode(GraphNode* node);

    GraphNode* &getStartNode();

    GraphNode* &getEndNode();

private:
    bool bStart = false;

    bool bEnd = false;

    bool bObstacle = false;

    GraphNode* start = nullptr;

    GraphNode* end = nullptr;

};

#endif // GRAPHICSSCENE_H
