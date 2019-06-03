#ifndef BALL_H
#define BALL_H

#include "objectsbase.h"
#include <QDebug>
#include <QColor>
#include <QGraphicsSceneMouseEvent>



class Ball : public Object
{
public:
    Ball();
    QColor myColor = Qt::yellow;
    int size = 50;//mm

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

};

#endif // BALL_H
