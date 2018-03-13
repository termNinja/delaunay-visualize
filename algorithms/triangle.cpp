#include "triangle.hpp"

#include <assert.h>
#include <math.h>

#include <QDebug>

#include "delaunay.hpp"

Triangle::Triangle()
    :
      p1(Vec2f(0, 0)),
      p2(Vec2f(1, 0)),
      p3(Vec2f(0, 1)),
      e1(p1, p2),
      e2(p2, p3),
      e3(p3, p1)
{
    initCircumCircle();
}

Triangle::Triangle(const Vec2f &_p1, const Vec2f &_p2, const Vec2f &_p3)
	:	p1(_p1), p2(_p2), p3(_p3),
		e1(_p1, _p2), e2(_p2, _p3), e3(_p3, _p1)
{
    initCircumCircle();
}

void Triangle::initCircumCircle()
{
    float ab = (p1.x * p1.x) + (p1.y * p1.y);
    float cd = (p2.x * p2.x) + (p2.y * p2.y);
    float ef = (p3.x * p3.x) + (p3.y * p3.y);

    float circum_x = (ab * (p3.y - p2.y) + cd * (p1.y - p3.y) + ef * (p2.y - p1.y)) / (p1.x * (p3.y - p2.y) + p2.x * (p1.y - p3.y) + p3.x * (p2.y - p1.y)) / 2.f;
    float circum_y = (ab * (p3.x - p2.x) + cd * (p1.x - p3.x) + ef * (p2.x - p1.x)) / (p1.y * (p3.x - p2.x) + p2.y * (p1.x - p3.x) + p3.y * (p2.x - p1.x)) / 2.f;

    circumCenter = Vec2f(circum_x, circum_y);
    circumRadius = sqrtf(((p1.x - circum_x) * (p1.x - circum_x)) + ((p1.y - circum_y) * (p1.y - circum_y)));
}

bool Triangle::containsVertex(const Vec2f &v) const
{
	return p1 == v || p2 == v || p3 == v; 
}

bool Triangle::circumCircleContains(const Vec2f &v)
{
	float ab = (p1.x * p1.x) + (p1.y * p1.y);
	float cd = (p2.x * p2.x) + (p2.y * p2.y);
	float ef = (p3.x * p3.x) + (p3.y * p3.y);

	float circum_x = (ab * (p3.y - p2.y) + cd * (p1.y - p3.y) + ef * (p2.y - p1.y)) / (p1.x * (p3.y - p2.y) + p2.x * (p1.y - p3.y) + p3.x * (p2.y - p1.y)) / 2.f;
	float circum_y = (ab * (p3.x - p2.x) + cd * (p1.x - p3.x) + ef * (p2.x - p1.x)) / (p1.y * (p3.x - p2.x) + p2.y * (p1.x - p3.x) + p3.y * (p2.x - p1.x)) / 2.f;
	float circum_radius = sqrtf(((p1.x - circum_x) * (p1.x - circum_x)) + ((p1.y - circum_y) * (p1.y - circum_y)));

	float dist = sqrtf(((v.x - circum_x) * (v.x - circum_x)) + ((v.y - circum_y) * (v.y - circum_y)));
	return dist <= circum_radius;
}

QString Triangle::str() const
{
    return QString::number(p1.x) + "," + QString::number(p1.y) + ","
         + QString::number(p2.x) + "," + QString::number(p2.y) + ","
         + QString::number(p3.x) + "," + QString::number(p3.y);

}
