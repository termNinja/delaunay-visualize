#ifndef GRAPHICSNODE_HPP
#define GRAPHICSNODE_HPP

#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QList>
#include <QPainter>

#include "gEdge.hpp"

#define NODE_TYPE 200

class MyGraphicsView;

class gNode : public QGraphicsItem
{
private:
    gEdge *m_e1, *m_e2;
    MyGraphicsView *m_graph;
    QList<gEdge*> m_edges;
    QColor m_color_clicked, m_color_inactive;
    bool m_current_node;

public:
    gNode(MyGraphicsView* g);
    gNode(MyGraphicsView* g, QColor clicked, QColor inactive);

    void addEdge(gEdge* edge);

    QString str() const;

    void setCurrentNode(bool b);
    bool currentNode() const;

    //! Defines the outer bounds of the item as rectangle.
    //! All painting needs to be restricted to inside an item's bounding rect.
    //! QGraphicsView uses this to determine whether the item requires redrawing.
    QRectF boundingRect() const Q_DECL_OVERRIDE;

    //! Paints the contents of an item in local coordinates.
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) Q_DECL_OVERRIDE;

    void setColor(QColor clicked, QColor inactive);

    //! Returns the shape of this item as a QPainterPath in local coordinates.
    //! The shape is used for many things, including collision detection, hit tests, and for the QGraphicsScene::items() functions.
    QPainterPath shape() const Q_DECL_OVERRIDE;

    //! Returns the type of an item as an int.
    //! All standard graphicsitem classes are associated with a unique value; see QGraphicsItem::Type.
    enum ItemType { TypeNode = UserType + 1 };
    int type() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    //! This virtual function is called by QGraphicsItem to notify custom items that some part of the item's state changes.
    //! By reimplementing this function, your can react to a change, and in some cases, (depending on change,) adjustments can be made.
    QVariant itemChange(GraphicsItemChange change, const QVariant& value);
};

#endif // GRAPHICSNODE_HPP
