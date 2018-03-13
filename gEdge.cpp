#include "gEdge.hpp"
#include "gNode.hpp"

#include <math.h>
#include <QPainter>
#include <QGraphicsSceneEvent>
#include <QGraphicsView>

#include "VisualisationColors.hpp"

gEdge::gEdge(gNode* start, gNode* end)
    : m_start(start), m_end(end)
{
    // Forbid the use of mouse on edges.
    setAcceptedMouseButtons(Qt::RightButton);

    // Force the drawing of edges BELOW the nodes.
    setZValue(-3);

    //m_color_clicked = QColor(255, 0, 0);
    //m_color_inactive = QColor(255, 0, 0);

    m_color_clicked = ITEM_EDGE_COLOR;
    m_color_inactive = ITEM_EDGE_COLOR;

    m_start->addEdge(this);
    m_end->addEdge(this);
    adjust();
}

gEdge::gEdge(gNode* start, gNode* end, QColor clicked, QColor inactive)
    : m_start(start), m_end(end), m_color_clicked(clicked), m_color_inactive(inactive)
{
    // Forbid the use of mouse on edges.
    setAcceptedMouseButtons(Qt::RightButton);

    // Force the drawing of edges BELOW the nodes.
    setZValue(-1);

    m_start->addEdge(this);
    m_end->addEdge(this);
    adjust();
}

gNode* gEdge::startNode()
{
    return m_start;
}

gNode* gEdge::endNode()
{
    return m_end;
}

void gEdge::adjust()
{
    prepareGeometryChange();

    m_sourcePoint = m_start->pos();
    m_destPoint = m_end->pos();
}

QRectF gEdge::boundingRect() const
{
    qreal w = m_destPoint.x() - m_sourcePoint.x();
    qreal h = m_destPoint.y() - m_sourcePoint.y();
    return QRectF(m_sourcePoint, QSizeF(w, h)).normalized();
}

void gEdge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //painter->setPen(QPen(QColor(70, 32, 102)));
    QPen p(m_color_inactive);
    p.setWidth(1);
    painter->setPen(p);
    painter->drawLine(m_sourcePoint, m_destPoint);
}

QString gEdge::str() const
{
    return QString::number(m_sourcePoint.x()) + "," + QString::number(m_sourcePoint.y()) + ","
            + QString::number(m_destPoint.x()) + "," + QString::number(m_destPoint.y());
}

void gEdge::setColor(QColor clicked, QColor inactive)
{
    m_color_clicked = clicked;
    m_color_inactive = inactive;
}

int gEdge::type() const
{
    return TypeEdge;
}

void gEdge::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->buttons() == Qt::RightButton) {
        scene()->removeItem(this);
    }
}
