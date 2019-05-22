#include "curve.h"

#include <assert.h>


Curve::Curve(const std::vector<Vec3> points, const float tension) :
    m_points{ points },
    m_tension{ tension }
{
}


Curve::~Curve()
{
}


Vec3 Curve::operator()(float t) const
{
    return { cos(t), sin(t), t };
    //DirectX::XMFLOAT3 point;
    //point.x = cos(t);
    //point.y = sin(t);
    //point.z = t;
    //return point;
}


Vec3 Curve::operator[](float t) const
{
    return { 0, 0, 0 };
}


Vec3 Curve::catmullRom(const float t, const float tension, const Vec3 p0, const Vec3 p1, const Vec3 p2, const Vec3 p3)
{
    assert(t >= 0.0f && t <= 1.0f);
    assert(tension >= 0.0f && tension <= 1.0f);

    float x = catmullRomPointMatrix(t, tension, p0.x, p1.x, p2.x, p3.x);
    float y = catmullRomPointMatrix(t, tension, p0.y, p1.y, p2.y, p3.y);
    float z = catmullRomPointMatrix(t, tension, p0.z, p1.z, p2.z, p3.z);

    return { x, y, z };
}


Vec3 Curve::catumullRomTangent(const float t, const float tension, const Vec3 p0, const Vec3 p1, const Vec3 p2, const Vec3 p3)
{
    assert(t >= 0.0f && t <= 1.0f);
    assert(tension >= 0.0f && tension <= 1.0f);

    float x = catmullRomTangentMatrix(t, tension, p0.x, p1.x, p2.x, p3.x);
    float y = catmullRomTangentMatrix(t, tension, p0.y, p1.y, p2.y, p3.y);
    float z = catmullRomTangentMatrix(t, tension, p0.z, p1.z, p2.z, p3.z);

    return { x, y, z };
}


float Curve::catmullRomPointMatrix(const float t, const float a, const float x0, const float x1, const float x2, const float x3)
{
    return (-a   *t*t*t + 2*a    *t*t - a*t    ) * x0 +
           ((2-a)*t*t*t + (a-3)  *t*t       + 1) * x1 +
           ((a-2)*t*t*t + (3-2*a)*t*t + a*t    ) * x2 +
           (a    *t*t*t - a      *t*t          ) * x3;  
}


float Curve::catmullRomTangentMatrix(const float t, const float a, const float x0, const float x1, const float x2, const float x3)
{
    return (-3*a   *t*t + 4*a      *t - a) * x0 +
           (3*(2-a)*t*t + 2*(a-3)  *t    ) * x1 +
           (3*(a-2)*t*t + 2*(3-2*a)*t + a) * x2 +
           (3*a    *t*t - 2*a      *t    ) * x3;
}


