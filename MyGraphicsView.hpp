#ifndef MYGRAPHICSVIEW_HPP
#define MYGRAPHICSVIEW_HPP

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QWheelEvent>
#include <qmath.h>
#include <QVector>
#include <QPointF>
#include <QPoint>
#include <QGraphicsItem>

#include <vector>

#include "gNode.hpp"
#include "gCircle.hpp"
#include "algorithms/vector2.hpp"
#include "algorithms/triangle.hpp"


// TODO: move this later
typedef Vector2<float> Vec2f;

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    MyGraphicsView(QWidget* parent = 0);
    void itemMoved();
    void createNode();
    void setPoints(std::vector<Vec2f> points);
    void setTriangles(QVector<Triangle>triangles);
    void finalSetOfTheTriangles(QVector<Triangle> triangles);
    void setTriangles(std::vector<Triangle> triangles);
    void setSuperTriangle(const Triangle & t);
    void removeEdges();

    void setCurrentTriangle(const Triangle & t);

    void updateCurrentPoint(const QPointF & p);

    void addCircle(const Triangle& t, const QPoint& center, float radius, const QPoint& vertex);
    void removeCircles();


    std::vector<Vec2f> points() const;
    const std::vector<Triangle>& triangles() const;

protected:
    void drawBackground(QPainter* painter, const QRectF& rect);
    void wheelEvent(QWheelEvent* event);

private:
    bool isPartOfSuperTriangle(const Triangle & t);

    std::vector<Vec2f> m_points;
    //std::vector<Triangle> m_triangles;
    std::vector<Triangle> m_triangles;
    Triangle m_super_triangle;

    std::vector<gCircle*> m_circles;

    QPointF m_current_point;
    bool m_current_point_exists;
};

#endif // MYGRAPHICSVIEW_HPP
