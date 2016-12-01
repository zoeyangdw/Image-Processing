#include "mypixmapitem.h"
#include <iostream>
using namespace std;

MyPixmapItem :: MyPixmapItem(QGraphicsPixmapItem *parent) :
    QGraphicsPixmapItem(parent)
{

}

void MyPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void MyPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    setPos(pos()+ mapToParent(event->pos())- mapToParent(event->lastPos()));
}

void MyPixmapItem::reset()
{
    setScale(1.0);
    setPos(0,0);
}

void MyPixmapItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    current_scale = scale();
    setTransformOriginPoint(event->pos());
    if (event->delta()> 0)
        setScale(scale()*1.2);
    else if (event->delta()< 0)
        setScale(scale()*0.8);
}

double MyPixmapItem::get_scale()
{
    return scale();
}
