#include "gNode.hpp"

#include "MyGraphicsView.hpp"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "VisualisationColors.hpp"

gNode::gNode(MyGraphicsView* g)
    : m_graph(g), m_current_node(false)
{
    // This allows for the QGraphicsItem to be moved using the mouse.
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);

    setZValue(10);

    setCacheMode(DeviceCoordinateCache);

    //m_color_clicked = QColor(255, 97, 97);
    //m_color_inactive = QColor(85, 255, 85);

    m_color_clicked = ITEM_NODE_COLOR_CLICKED;
    m_color_inactive = ITEM_NODE_COLOR;
}

gNode::gNode(MyGraphicsView* g, QColor clicked, QColor inactive)
    : m_graph(g), m_color_clicked(clicked), m_color_inactive(inactive), m_current_node(false)
{
    // This allows for the QGraphicsItem to be moved using the mouse.
    setFlag(ItemIsMovable);

    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
}

void gNode::addEdge(gEdge* edge)
{
    m_edges.push_back(edge);
}

QString gNode::str() const
{
    return QString::number(x()) + "," + QString::number(y());
}

void gNode::setCurrentNode(bool b)
{
    m_current_node = b;
    update();
}

bool gNode::currentNode() const
{
    return m_current_node;
}

QRectF gNode::boundingRect() const
{
    return QRect(-12, -12, 25, 25);
}

void gNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget);

    QBrush brush;
    QPen pen;

    brush.setStyle(Qt::SolidPattern);

    // Draw our item with an appropriate color.
    if (option->state & QStyle::State_Sunken) {
        brush.setColor(m_color_clicked);
        pen.setColor(m_color_clicked);
    } else {
        brush.setColor(m_color_inactive);
        pen.setColor(m_color_inactive);
    }

    if (m_current_node) {
        brush.setColor(ITEM_NODE_ACTIVE_COLOR);
        pen.setColor(ITEM_NODE_ACTIVE_COLOR);
    }

    painter->setBrush(brush);
    painter->setPen(pen);
    //painter->drawEllipse(-10, -10, 20, 20);
    painter->drawEllipse(-5, -5, 10, 10);
}

void gNode::setColor(QColor clicked, QColor inactive)
{
    m_color_clicked = clicked;
    m_color_inactive = inactive;
}

QPainterPath gNode::shape() const
{
    QPainterPath p;
    p.addEllipse(-10, -10, 20, 20);
    return p;
}

int gNode::type() const
{
    return TypeNode;
}

void gNode::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    update();
    if (event->buttons() == Qt::RightButton) {
        qDebug() << "Right button was pressed, deleting object.";
        scene()->removeItem(this);
    }
    QGraphicsItem::mousePressEvent(event);
}

void gNode::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

QVariant gNode::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionHasChanged) {
        foreach (gEdge* e, m_edges)
            e->adjust();
        // TODO: Is this needed?
        m_graph->itemMoved();
    }

    return QGraphicsItem::itemChange(change, value);
}
