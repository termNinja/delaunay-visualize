#include "gCircle.hpp"

#include <QPainter>

gCircle::gCircle(float radius)
    : m_radius(radius)
{
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-2);
}

QRectF gCircle::boundingRect() const
{
    int penWidth = 1;
    return QRectF(-m_radius - penWidth / 2, -m_radius - penWidth / 2,
                      m_radius*2 + penWidth, m_radius*2 + penWidth);
}

void gCircle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    QBrush brush(QColor(255, 255, 0));
    QPen pen(QColor(255, 255, 0));

    painter->setPen(pen);
    painter->drawEllipse(-m_radius, -m_radius, m_radius*2, m_radius*2);
}

int gCircle::type() const
{
    return TypeCircle;
}
