#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <iostream>
#include <cmath>

template <typename T>
class Vector2
{
public:
    Vector2()
    {
        x = 0;
        y = 0;
    }

    Vector2(T _x, T _y)
    {
        x = _x;
        y = _y;
    }

    Vector2(const Vector2 &v)
    {
        x = v.x;
        y = v.y;
    }

    void set(const Vector2 &v)
    {
        x = v.x;
        y = v.y;
    }

    T dist2(const Vector2 &v)
    {
        T dx = x - v.x;
        T dy = y - v.y;
        return dx * dx + dy * dy;
    }

    float dist(const Vector2 &v)
    {
        return sqrtf(dist2(v));
    }

    T x;
    T y;

};

template<typename T>
std::ostream &operator << (std::ostream &str, Vector2<T> const &point) 
{
    return str << "Point x: " << point.x << " y: " << point.y;
}

template<typename T>
bool operator == (Vector2<T> v1, Vector2<T> v2)
{
    return (v1.x == v2.x) && (v1.y == v2.y);
}

#endif // VECTOR2_HPP
