#ifndef DELAUNAY_HPP
#define DELAUNAY_HPP

#include <vector>

#include <QThread>
#include <QString>
#include <QVector>

#include "vector2.hpp"
#include "triangle.hpp"

#include <QMutex>
#include <QPointF>
#include <QWaitCondition>

float RandomFloat(float a, float b);


class Delaunay : public QThread
{
    Q_OBJECT
public:
    Delaunay(bool animation, unsigned long thread_sleep = 0);
    Delaunay(bool animation, std::vector<Vec2f>& vertices, unsigned long thread_sleep = 0);
    const std::vector<Triangle>& triangulate();
    const std::vector<Triangle>& triangulate_without_animation(std::vector<Vec2f> &vertices);
    const std::vector<Triangle>& getTriangles() const;
    const std::vector<Edge>& getEdges() const;
    const std::vector<Vec2f>& getVertices() const;

    void pauseIfNeeded();
    void pause();
    void play();

    void setThreadSleep(unsigned long thread_sleep);

signals:
    void updateMessage(QString message);
    void superTriangle(const Triangle & t);
    //void updatedTriangles(const std::vector<Triangle> & triangles);
    //void updatedTriangles(const QVector<Triangle> & triangles);
    void updatedTriangles(QVector<Triangle>);
    void done(QVector<Triangle>);
    void reportPercentage(unsigned percent);
    void showCircle(const Triangle & t, const QPoint & center, float radius, const QPoint & vertex);
    void clearCircles();
    void updateCurrentPoint(const QPointF & p);
    void reportTriangle(const Triangle & t);


protected:
    void run();

private:
    void reportCircle(const Triangle & t, const Vec2f & p);
    void removeSuperTriangle(Vec2f p1, Vec2f p2, Vec2f p3);
    Triangle findSuperTriangle();

    std::vector<Triangle> m_triangles;
    std::vector<Edge> m_edges;
    std::vector<Vec2f> m_vertices;
    unsigned long m_thread_sleep;

    // Used for allowing for play/pause mechanism.
    QWaitCondition m_wait_cond;
    QMutex m_mutex;

    bool m_pause;
    bool m_animation;
};

#endif // DELAUNAY_HPP
