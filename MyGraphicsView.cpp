#include "MyGraphicsView.hpp"

#include <QDebug>
#include <set>


#include "algorithms/edge.hpp"
#include "algorithms/vector2.hpp"
#include "algorithms/triangle.hpp"

#include "VisualisationColors.hpp"

typedef Vector2<float> Vec2f;

MyGraphicsView::MyGraphicsView(QWidget* parent)
    : QGraphicsView(parent)
{
    QGraphicsScene* scene = new QGraphicsScene(this);
    // scene->setSceneRect(-500, -200, 400, 400);
    scene->setSceneRect(-3000, -3000, 6000, 6000);

    setScene(scene);
    //setMinimumSize(500, 500);
    setRenderHint(QPainter::Antialiasing);

    // TODO: Add scene items here
    gNode *n1 = new gNode(this);
    gNode *n2 = new gNode(this);
    gNode *n3 = new gNode(this);

    scene->addItem(n1);
    scene->addItem(n2);
    scene->addItem(n3);

    //this->setDragMode(DragMode::);

    Vec2f v1(40, 0);
    n1->setPos(40, 0);
    Vec2f v2(40, 40);
    n2->setPos(40, 40);
    Vec2f v3(80, 80);
    n3->setPos(80, 80);

    std::vector<Vec2f> points = { v1, v2, v3 };
    m_points = points;

    //scene->addItem(new gEdge(n1, n2));
    //scene->addItem(new gEdge(n3, n2));
}

void MyGraphicsView::itemMoved()
{
}

void MyGraphicsView::createNode()
{
    gNode* n = new gNode(this);
    n->setPos(0, 0);
    scene()->addItem(n);
}

void MyGraphicsView::setPoints(std::vector<Vec2f> points)
{
    m_points = points;
    m_current_point_exists = false;


    qDebug() << "Clearning scene elements.";
    qDebug() << "Adding " << points.size() << " elements to scene.";
    scene()->clear();
    for (const auto & p : m_points) {
        gNode* n = new gNode(this);
        n->setPos(p.x, p.y);
        scene()->addItem(n);
    }
}

void MyGraphicsView::setTriangles(QVector<Triangle> triangles)
{
    m_triangles = triangles.toStdVector();

    // Remove all previous edges.
    for (auto & e : scene()->items()) {
        if (e->type() == gEdge::TypeEdge)
            scene()->removeItem(e);
        else if (e->type() == QGraphicsPolygonItem::Type)
            scene()->removeItem(e);
    }

    std::set<Edge> edges;
    for (auto & t : m_triangles) {
        edges.insert(t.e1);
        edges.insert(t.e2);
        edges.insert(t.e3);
    }

    for (auto & e : edges) {
        auto item1 = scene()->itemAt(e.p1.x, e.p1.y, QGraphicsView::transform());
        auto item2 = scene()->itemAt(e.p2.x, e.p2.y, QGraphicsView::transform());
        if (item1->type() == item2->type() && item1->type() == gNode::TypeNode) {
            gNode* n1 = static_cast<gNode*>(item1);
            gNode* n2 = static_cast<gNode*>(item2);
            gEdge* gedge = new gEdge(n1, n2);
            scene()->addItem(gedge);
        }
    }
}

void MyGraphicsView::finalSetOfTheTriangles(QVector<Triangle> triangles)
{
    // remove super triangle vertices
    gNode* n1 = static_cast<gNode*>(scene()->itemAt(m_super_triangle.p1.x, m_super_triangle.p1.y, QGraphicsView::transform()));
    gNode* n2 = static_cast<gNode*>(scene()->itemAt(m_super_triangle.p2.x, m_super_triangle.p2.y, QGraphicsView::transform()));
    gNode* n3 = static_cast<gNode*>(scene()->itemAt(m_super_triangle.p3.x, m_super_triangle.p3.y, QGraphicsView::transform()));
    if (n1 != nullptr) scene()->removeItem(n1);
    if (n2 != nullptr) scene()->removeItem(n2);
    if (n3 != nullptr) scene()->removeItem(n3);

    setTriangles(triangles);

    // Make last active node inactive
    if (m_current_point_exists) {
        auto i = scene()->itemAt(m_current_point.x(), m_current_point.y(), QGraphicsView::transform());
        if (i->type() == gNode::TypeNode)
            static_cast<gNode*>(i)->setCurrentNode(false);
    }
}

void MyGraphicsView::setTriangles(std::vector<Triangle> triangles)
{
    qDebug() << "Setting triangles to canvas...";
    m_triangles = triangles;

    std::set<Edge> edges;
    for (auto & t : m_triangles) {
        edges.insert(t.e1);
        edges.insert(t.e2);
        edges.insert(t.e3);
    }

    for (auto & e : edges) {
        gNode* n1 = static_cast<gNode*>(scene()->itemAt(e.p1.x, e.p1.y, QGraphicsView::transform()));
        gNode* n2 = static_cast<gNode*>(scene()->itemAt(e.p2.x, e.p2.y, QGraphicsView::transform()));
        gEdge* gedge = new gEdge(n1, n2);
        scene()->addItem(gedge);
    }
}

void MyGraphicsView::setSuperTriangle(const Triangle& t)
{
    m_super_triangle = t;

    QColor nodeColor = ITEM_SUPER_TRIANGLE_COLOR;
    QColor edgeColor = ITEM_SUPER_TRIANGLE_COLOR;

    gNode* n1 = new gNode(this, nodeColor, nodeColor);
    gNode* n2 = new gNode(this, nodeColor, nodeColor);
    gNode* n3 = new gNode(this, nodeColor, nodeColor);
    n1->setPos(QPoint(t.p1.x, t.p1.y));
    n2->setPos(QPoint(t.p2.x, t.p2.y));
    n3->setPos(QPoint(t.p3.x, t.p3.y));
    gEdge* e1 = new gEdge(n1, n2, edgeColor, edgeColor);
    gEdge* e2 = new gEdge(n2, n3, edgeColor, edgeColor);
    gEdge* e3 = new gEdge(n3, n1, edgeColor, edgeColor);
    scene()->addItem(n1);
    scene()->addItem(n2);
    scene()->addItem(n3);
    scene()->addItem(e1);
    scene()->addItem(e2);
    scene()->addItem(e3);
}

void MyGraphicsView::removeEdges()
{
    auto stuffs = scene()->items();
    for (auto & stuff : stuffs)
        if (stuff->type() == gEdge::TypeEdge)
            scene()->removeItem(stuff);
        else if (stuff->type() == QGraphicsPolygonItem::Type)
            scene()->removeItem(stuff);

    m_triangles.clear();
}

void MyGraphicsView::setCurrentTriangle(const Triangle& t)
{
    if (isPartOfSuperTriangle(t))
        return;

    QPolygonF x;
    x.append(QPointF(t.p1.x, t.p1.y));
    x.append(QPointF(t.p2.x, t.p2.y));
    x.append(QPointF(t.p3.x, t.p3.y));
    x.append(QPointF(t.p1.x, t.p1.y));

    // Add the triangle polygon to the scene
    QGraphicsPolygonItem* pTriangleItem = scene()->addPolygon(x);
    pTriangleItem->setZValue(-1);
    QPen p(ITEM_BAD_TRIANGLE_COLOR);
    p.setWidth(3);
    pTriangleItem->setPen(p);
}

void MyGraphicsView::updateCurrentPoint(const QPointF& p)
{
    if (m_current_point_exists) {
        // Set the current one as not active one
        auto t = scene()->itemAt(m_current_point.x(), m_current_point.y(), QGraphicsView::transform());
        if (t->type() == gNode::TypeNode) {
            static_cast<gNode*>(t)->setCurrentNode(false);
            //qDebug() << "Succesffuly grabed m_current_point";
        }
        else {
            qDebug() << "MyGraphicsView::updateCurrentPoint has selected something that"
                     << "should've been a gNode* but isn't...(first call)"
                     << "Tried point at: " << m_current_point;
            return;
        }
    }

    // Set the new point as active one
    auto t = scene()->itemAt(p.x(), p.y(), QGraphicsView::transform());
    if (t->type() == gNode::TypeNode) {
        static_cast<gNode*>(t)->setCurrentNode(true);
        m_current_point_exists = true;
        m_current_point = QPointF(t->x(), t->y());
    }
    else {
        qDebug() << "MyGraphicsView::updateCurrentPoint has selected something that"
                 << "should've been a gNode* but isn't...(second call)";
        return;
    }
}

void MyGraphicsView::addCircle(const Triangle& t, const QPoint& center, float radius, const QPoint& vertex)
{

    if (! isPartOfSuperTriangle(t)) {
        gCircle* c = new gCircle(radius);
        m_circles.push_back(c);
        c->setPos(center);
        scene()->addItem(c);
    }

    // We won't visualize super triangle circles.
   // if (!t.containsVertex(m_super_triangle.p1) &&
   //     !t.containsVertex(m_super_triangle.p2) &&
   //     !t.containsVertex(m_super_triangle.p3))
   //     m_circles.push_back(c);
   // c->setPos(center);
   // scene()->addItem(c);
}

void MyGraphicsView::removeCircles()
{
    for (auto & e : scene()->items())
        if (e->type() == gCircle::TypeCircle)
            scene()->removeItem(e);
}

std::vector<Vec2f> MyGraphicsView::points() const
{
    return m_points;
}

const std::vector<Triangle>&MyGraphicsView::triangles() const
{
    return m_triangles;
}

void MyGraphicsView::drawBackground(QPainter* painter, const QRectF& rect1)
{
    Q_UNUSED(rect1);

    QRectF rect = this->sceneRect();
    painter->drawRect(rect);
}

void MyGraphicsView::wheelEvent(QWheelEvent* event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    double scaleFactor = 1.15;

    if (event->delta() > 0)
        scale(scaleFactor, scaleFactor);
    else
        scale(1/scaleFactor, 1/scaleFactor);
}

bool MyGraphicsView::isPartOfSuperTriangle(const Triangle& t)
{
    if (m_super_triangle.containsVertex(t.p1) || m_super_triangle.containsVertex(t.p2) || m_super_triangle.containsVertex(t.p3))
        return true;
    else
        return false;
}
