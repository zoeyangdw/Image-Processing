#ifndef MYPIXMAPITEM_H
#define MYPIXMAPITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>

class MyPixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit MyPixmapItem(QGraphicsPixmapItem *parent = 0);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void reset();
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    double get_scale();
    double current_scale;

signals:

public slots:

};

#endif // MYPIXMAPITEM_H
