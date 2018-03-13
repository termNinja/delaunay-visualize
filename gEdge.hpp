#ifndef GEDGE_HPP
#define GEDGE_HPP

#include <QGraphicsItem>

// Forward declare node in order to make both classes have points to each other.
class gNode;

class gEdge : public QGraphicsItem
{
public:
    gEdge(gNode *start, gNode *end);
    gEdge(gNode *start, gNode *end, QColor clicked, QColor inactive);

    ~gEdge() {}

    gNode* startNode();
    gNode* endNode();

    void adjust();

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    QString str() const;

    void setColor(QColor clicked, QColor inactive);

    enum ItemType { TypeEdge = UserType + 2 };
    int type() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

private:
    gNode *m_start, *m_end;
    QPointF m_sourcePoint, m_destPoint;
    QColor m_color_clicked, m_color_inactive;
};

#endif // GEDGE_HPP
