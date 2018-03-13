#ifndef GCIRCLE_HPP
#define GCIRCLE_HPP

#include <QGraphicsItem>

class gCircle : public QGraphicsItem
{
public:
    gCircle(float radius);

    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    enum ItemType { TypeCircle = UserType + 3 };
    int type() const;

private:
    float m_radius;

};

#endif // GCIRCLE_HPP
