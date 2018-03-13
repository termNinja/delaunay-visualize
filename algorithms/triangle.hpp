#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "vector2.hpp"
#include "edge.hpp"
#include <string>
#include <QString>

class Triangle
{
public:
    Triangle();
    Triangle(const Vec2f &_p1, const Vec2f &_p2, const Vec2f &_p3);

    bool containsVertex(const Vec2f &v) const;
    bool circumCircleContains(const Vec2f &v);

    //std::string str() const;
    QString str() const;

    Vec2f p1;
    Vec2f p2;
    Vec2f p3;
    Edge e1;
    Edge e2;
    Edge e3;
    Vec2f circumCenter;
    float circumRadius;

private:
    void initCircumCircle();
};

inline std::ostream &operator << (std::ostream &str, const Triangle & t)
{
    return str << "Triangle:" << std::endl << "\t" << t.p1 << std::endl << "\t" << t.p2 << std::endl << "\t" << t.p3 << std::endl << "\t" << t.e1 << std::endl << "\t" << t.e2 << std::endl << "\t" << t.e3 << std::endl;

}

inline bool operator == (const Triangle &t1, const Triangle &t2)
{
    return	(t1.p1 == t2.p1 || t1.p1 == t2.p2 || t1.p1 == t2.p3) &&
            (t1.p2 == t2.p1 || t1.p2 == t2.p2 || t1.p2 == t2.p3) &&
            (t1.p3 == t2.p1 || t1.p3 == t2.p2 || t1.p3 == t2.p3);
}

inline bool operator != (const Triangle &t1, const Triangle &t2)
{
    return	! (t1 == t2);
}


#endif // TRIANGLE_HPP
