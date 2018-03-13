#ifndef EDGE_HPP
#define EDGE_HPP

#include "vector2.hpp"

#include <QtMath>

typedef Vector2<float> Vec2f;

class Edge
{
public:
    Edge(const Vec2f &p1, const Vec2f &p2) : p1(p1), p2(p2)
    {
    }
    Edge(const Edge &e) : p1(e.p1), p2(e.p2)
    {
    }

    Vec2f p1;
    Vec2f p2;
};

inline std::ostream& operator << (std::ostream &str, Edge const &e)
{
    return str << "Edge " << e.p1 << ", " << e.p2;
}

inline bool operator == (const Edge & e1, const Edge & e2)
{
    return 	(e1.p1 == e2.p1 && e1.p2 == e2.p2) ||
            (e1.p1 == e2.p2 && e1.p2 == e2.p1);
}

#include <iostream>
inline bool operator < (const Edge & e1, const Edge & e2)
{
    double alpha = qRadiansToDegrees(qAtan2(e1.p1.y - e1.p2.y, e1.p1.x - e1.p2.x));
    double beta = qRadiansToDegrees(qAtan2(e2.p1.y - e2.p2.y, e2.p1.x - e2.p2.x));

    //std::cout << "---------------------------------------\n";
    //std::cout << "edge::operator<    alpha: " << alpha << "   beta: " << beta << std::endl;
    //std::cout << "---------------------------------------\n";

    if (alpha < beta) return true;
    else return false;
}

#endif // EDGE_HPP
