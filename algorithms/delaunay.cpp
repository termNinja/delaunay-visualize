#include "delaunay.hpp"
#include <algorithm>

#include <QPoint>
#include <QDebug>

#define THREAD_SLEEP() if (m_thread_sleep > 0) QThread::msleep(m_thread_sleep); pauseIfNeeded();

QString point_to_string(const Vec2f & p)
{
    return "(" + QString::number(p.x) + ", " + QString::number(p.y) + ")";
}

QString vector_to_string(const Vec2f & v)
{
    return "(" + QString::number(v.x) + ", " + QString::number(v.y) + ")";
}

QString triangle_to_string(const Vec2f & v1, const Vec2f & v2, const Vec2f & v3)
{
    return vector_to_string(v1) + " " + vector_to_string(v2) + " " + vector_to_string(v3);
}

QString triangle_to_string(const Triangle & t)
{
    return vector_to_string(t.p1) + " " + vector_to_string(t.p2) + " " + vector_to_string(t.p3);
}

float RandomFloat(float a, float b)
{
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

Delaunay::Delaunay(bool animation, unsigned long thread_sleep)
    : m_thread_sleep(thread_sleep), m_pause(false), m_animation(animation)
{
}

Delaunay::Delaunay(bool animation, std::vector<Vec2f>& vertices, unsigned long thread_sleep)
    : m_vertices(vertices), m_thread_sleep(thread_sleep), m_pause(false), m_animation(animation)
{
}

void Delaunay::reportCircle(const Triangle & t, const Vec2f & p)
{
    //qDebug() << "Circle at" << circum_x << circum_y << "with radius of" << circum_radius;
    emit showCircle(t, QPoint(t.circumCenter.x, t.circumCenter.y), t.circumRadius, QPoint(p.x, p.y));
}

void Delaunay::removeSuperTriangle(Vec2f p1, Vec2f p2, Vec2f p3)
{
    m_triangles.erase(
        std::remove_if(begin(m_triangles), end(m_triangles),
            [p1, p2, p3](Triangle &t){
                return t.containsVertex(p1) || t.containsVertex(p2) || t.containsVertex(p3);
            }
    ), end(m_triangles));
}

Triangle Delaunay::findSuperTriangle()
{
    float minX = m_vertices[0].x;
    float minY = m_vertices[0].y;
    float maxX = minX;
    float maxY = minY;

    for (std::size_t i = 0; i < m_vertices.size(); ++i) {
        if (m_vertices[i].x < minX) minX = m_vertices[i].x;
        if (m_vertices[i].y < minY) minY = m_vertices[i].y;
        if (m_vertices[i].x > maxX) maxX = m_vertices[i].x;
        if (m_vertices[i].y > maxY) maxY = m_vertices[i].y;
    }

    float dx = maxX - minX;
    float dy = maxY - minY;
    float deltaMax = std::max(dx, dy);
    float midx = (minX + maxX) / 2.f;
    float midy = (minY + maxY) / 2.f;

    Vec2f p1(midx - 20 * deltaMax, midy - deltaMax);
    Vec2f p2(midx, midy + 20 * deltaMax);
    Vec2f p3(midx + 20 * deltaMax, midy - deltaMax);

    return Triangle(p1, p2, p2);
}

const std::vector<Triangle>& Delaunay::triangulate()
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // Determinate the super triangle
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    // Create a list of triangles, and add the supertriangle in it
    float minX = m_vertices[0].x;
    float minY = m_vertices[0].y;
    float maxX = minX;
    float maxY = minY;

    for (std::size_t i = 0; i < m_vertices.size(); ++i) {
        if (m_vertices[i].x < minX) minX = m_vertices[i].x;
        if (m_vertices[i].y < minY) minY = m_vertices[i].y;
        if (m_vertices[i].x > maxX) maxX = m_vertices[i].x;
        if (m_vertices[i].y > maxY) maxY = m_vertices[i].y;
    }

    float dx = maxX - minX;
    float dy = maxY - minY;
    float deltaMax = std::max(dx, dy);
    float midx = (minX + maxX) / 2.f;
    float midy = (minY + maxY) / 2.f;

    Vec2f p1(midx - 20 * deltaMax, midy - deltaMax);
    Vec2f p2(midx, midy + 20 * deltaMax);
    Vec2f p3(midx + 20 * deltaMax, midy - deltaMax);

    Triangle t(p1, p2, p3);
    m_triangles.push_back(t);

    // -----------------------------------◖(◣☩◢)◗-----------------------------------
    emit updateMessage("Determined super triangle at: " + triangle_to_string(t));
    emit superTriangle(t);
    emit updateMessage("Starting iteration over vertices.");
    THREAD_SLEEP();
    // -----------------------------------◖(◣☩◢)◗-----------------------------------

    unsigned main_iteration_counter = 0;
    unsigned total_iterations = m_vertices.size();
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // FOR ALL VERTICES
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    for (auto p = begin(m_vertices); p != end(m_vertices); p++) {

        // -----------------------------------◖(◣☩◢)◗-----------------------------------
        if (m_animation) {
            emit updateCurrentPoint(QPointF(p->x, p->y));
            emit updateMessage("Currently at point: " + point_to_string(*p));
            emit updateMessage("Searching for bad triangles...");
            THREAD_SLEEP();
        }
        // -----------------------------------◖(◣☩◢)◗-----------------------------------

        std::vector<Triangle> badTriangles;
        std::vector<Edge> polygon;

        // FOR ALL TRIANGLES IN TRIANGULATION
        for (auto t = begin(m_triangles); t != end(m_triangles); t++) {

            if (t->circumCircleContains(*p)) {

                // -----------------------------------◖(◣☩◢)◗-----------------------------------
                if (m_animation) {
                    reportCircle(*t, *p);
                    emit reportTriangle(*t);
                    THREAD_SLEEP();
                }
                // -----------------------------------◖(◣☩◢)◗-----------------------------------

                badTriangles.push_back(*t);
                polygon.push_back(t->e1);
                polygon.push_back(t->e2);
                polygon.push_back(t->e3);
            } else {
                //std::cout << " does not contains " << *p << " in his circum center" << std::endl;
            }

        }

        // -----------------------------------◖(◣☩◢)◗-----------------------------------
        if (m_animation) {
            emit clearCircles();
            emit updateMessage("Done searching for bad triangles...");
            emit updateMessage("Removing bad triangles from triangulation...");
        }
        // -----------------------------------◖(◣☩◢)◗-----------------------------------

        // Remove bad triangles from triangulation.
        m_triangles.erase(std::remove_if(begin(m_triangles), end(m_triangles), [this, badTriangles](Triangle &t){
            for (auto bt = begin(badTriangles); bt != end(badTriangles); bt++) {
                if (*bt == t) {
//                    emit updateMessage("Removing bad triangle: " + triangle_to_string(*bt));
                    //std::cout << "Removing bad triangle " << std::endl << *bt << " from _triangles" << std::endl;
                    return true;
                }
            }
            return false;
        }), end(m_triangles));

        // -----------------------------------◖(◣☩◢)◗-----------------------------------
        if (m_animation) {
            QVector<Triangle> tmp = QVector<Triangle>::fromStdVector(m_triangles);
            emit updateMessage("Done removing bad triangles from triangulation.");
            emit updateMessage("Finding bad edges and removing them from polygon...");
            emit updatedTriangles(tmp);
            THREAD_SLEEP();
        }
        // -----------------------------------◖(◣☩◢)◗-----------------------------------

        std::vector<Edge> badEdges;
        for (auto e1 = begin(polygon); e1 != end(polygon); e1++) {
            for (auto e2 = begin(polygon); e2 != end(polygon); e2++) {
                if (e1 == e2)
                    continue;

                if (*e1 == *e2) {
                    badEdges.push_back(*e1);
                    badEdges.push_back(*e2);
                }
            }
        }

        polygon.erase(std::remove_if(begin(polygon), end(polygon), [badEdges](Edge &e){
            for (auto it = begin(badEdges); it != end(badEdges); it++) {
                if(*it == e)
                    return true;
            }
            return false;
        }), end(polygon));

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // Retriangulate, add triangles to triangulation that are formed
        // from edges in polygon and current point
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        for (auto e = begin(polygon); e != end(polygon); e++) {
            Triangle t(e->p1, e->p2, *p);
            m_triangles.push_back(t);

            // -----------------------------------◖(◣☩◢)◗-----------------------------------
            if (m_animation) {
                QVector<Triangle> tmp = QVector<Triangle>::fromStdVector(m_triangles);
                //emit updateMessage("Adding triangle: " + triangle_to_string(t));
                emit updatedTriangles(tmp);
                THREAD_SLEEP();
            }
            // -----------------------------------◖(◣☩◢)◗-----------------------------------
        }

        if (m_animation) {
            emit updateMessage("Retriangulated hole left by removed edges.");
        }

        // Report percentage
        main_iteration_counter++;
        emit reportPercentage(floor(main_iteration_counter*1.0/total_iterations*100.0));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // Remove super triangle
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    removeSuperTriangle(t.p1, t.p2, t.p3);

    for (auto t = begin(m_triangles); t != end(m_triangles); t++) {
        m_edges.push_back(t->e1);
        m_edges.push_back(t->e2);
        m_edges.push_back(t->e3);
    }

    // -----------------------------------◖(◣☩◢)◗-----------------------------------
    emit updateMessage("Algorithm has completed.");
    emit done(QVector<Triangle>::fromStdVector(m_triangles));
    THREAD_SLEEP();
    // -----------------------------------◖(◣☩◢)◗-----------------------------------

    return m_triangles;
}

const std::vector<Triangle>& Delaunay::triangulate_without_animation(std::vector<Vec2f> &vertices)
{
    // Store the vertices localy
    m_vertices = vertices;

    // Determinate the super triangle
    float minX = vertices[0].x;
    float minY = vertices[0].y;
    float maxX = minX;
    float maxY = minY;

    for (std::size_t i = 0; i < vertices.size(); ++i) {
        if (vertices[i].x < minX) minX = vertices[i].x;
        if (vertices[i].y < minY) minY = vertices[i].y;
        if (vertices[i].x > maxX) maxX = vertices[i].x;
        if (vertices[i].y > maxY) maxY = vertices[i].y;
    }

    float dx = maxX - minX;
    float dy = maxY - minY;
    float deltaMax = std::max(dx, dy);
    float midx = (minX + maxX) / 2.f;
    float midy = (minY + maxY) / 2.f;

    Vec2f p1(midx - 20 * deltaMax, midy - deltaMax);
    Vec2f p2(midx, midy + 20 * deltaMax);
    Vec2f p3(midx + 20 * deltaMax, midy - deltaMax);

    //std::cout << "Super triangle " << std::endl << Triangle(p1, p2, p3) << std::endl;

    // Create a list of triangles, and add the supertriangle in it
    m_triangles.push_back(Triangle(p1, p2, p3));

    for (auto p = begin(vertices); p != end(vertices); p++) {
        //std::cout << "Treating point " << *p << std::endl;
        //std::cout << "_triangles contains " << _triangles.size() << " elements" << std::endl;

        std::vector<Triangle> badTriangles;
        std::vector<Edge> polygon;

        for (auto t = begin(m_triangles); t != end(m_triangles); t++) {
            //std::cout << "Processing " << std::endl << *t << std::endl;

            if (t->circumCircleContains(*p)) {
                //std::cout << "Pushing bad triangle " << *t << std::endl;
                badTriangles.push_back(*t);
                polygon.push_back(t->e1);
                polygon.push_back(t->e2);
                polygon.push_back(t->e3);
            } else {
                //std::cout << " does not contains " << *p << " in his circum center" << std::endl;
            }
        }

        m_triangles.erase(std::remove_if(begin(m_triangles), end(m_triangles), [badTriangles](Triangle &t){
            for (auto bt = begin(badTriangles); bt != end(badTriangles); bt++) {
                if (*bt == t) {
                    //std::cout << "Removing bad triangle " << std::endl << *bt << " from _triangles" << std::endl;
                    return true;
                }
            }
            return false;
        }), end(m_triangles));

        std::vector<Edge> badEdges;
        for (auto e1 = begin(polygon); e1 != end(polygon); e1++) {
            for (auto e2 = begin(polygon); e2 != end(polygon); e2++) {
                if (e1 == e2)
                    continue;

                if (*e1 == *e2) {
                    badEdges.push_back(*e1);
                    badEdges.push_back(*e2);
                }
            }
        }

        polygon.erase(std::remove_if(begin(polygon), end(polygon), [badEdges](Edge &e){
            for (auto it = begin(badEdges); it != end(badEdges); it++) {
                if(*it == e)
                    return true;
            }
            return false;
        }), end(polygon));

        for (auto e = begin(polygon); e != end(polygon); e++)
            m_triangles.push_back(Triangle(e->p1, e->p2, *p));

    }

    m_triangles.erase(std::remove_if(begin(m_triangles), end(m_triangles), [p1, p2, p3](Triangle &t){
        return t.containsVertex(p1) || t.containsVertex(p2) || t.containsVertex(p3);
    }), end(m_triangles));

    for (auto t = begin(m_triangles); t != end(m_triangles); t++) {
        m_edges.push_back(t->e1);
        m_edges.push_back(t->e2);
        m_edges.push_back(t->e3);
    }

    return m_triangles;
}

const std::vector<Triangle>& Delaunay::getTriangles() const
{
    return m_triangles;
}

const std::vector<Edge>& Delaunay::getEdges() const
{
    return m_edges;
}

const std::vector<Vec2f>& Delaunay::getVertices() const
{
    return m_vertices;
}

void Delaunay::pauseIfNeeded()
{
    // Qt make QWaitCondition in such a way that it requires a locked mutex
    // to be passed as argument, even though mutex wasn't needed here.
    if (m_pause) {
        m_mutex.lock();
        m_wait_cond.wait(&m_mutex);
        m_mutex.unlock();
    }
}

void Delaunay::pause()
{
    m_pause = true;
}

void Delaunay::play()
{
    m_wait_cond.wakeOne();
    m_pause = false;
}

void Delaunay::setThreadSleep(unsigned long thread_sleep)
{
    m_thread_sleep = thread_sleep;
}

void Delaunay::run()
{
    triangulate();
}
