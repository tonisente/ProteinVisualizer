#include "MathLib.h"


Vec3::Vec3()
{}


Vec3::Vec3(const float a, const float b, const float c) :
    x{ a },
    y{ b },
    z{ c }
{
}


Vec3 Vec3::operator=(const Vec3& other)
{
    x = other.x;
    y = other.y;
    z = other.z;

    return *this;
}


float operator*(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 operator*(const Vec3& v, const float lambda)
{
    return { v.x * lambda, v.y * lambda, v.z * lambda };
}

Vec3 operator*(const float lambda, const Vec3& v)
{
    return v * lambda;
}

Vec3 operator/(const Vec3& v, const float lambda)
{
    return v * (1 / lambda);
}

Vec3 operator/(const float lambda, const Vec3&v)
{
    return lambda / v;
}

Vec3 Vec3::operator+(const Vec3& other) const
{
    return { x + other.x, y + other.y, z + other.z };
}

Vec3 Vec3::operator-(const Vec3& other) const
{
    return { x - other.x, y - other.y, z - other.z };
}

Vec3 Vec3::normalized() const
{
    float param = std::sqrt(x * x + y * y + z * z);
    return { x / param, y / param, z / param };
}

Vec3 Vec3::cross(const Vec3& a, const Vec3& b)
{
    return
    {
        a.y * b.z - a.z * b.y,
        -(a.x * b.z - a.z * b.x),
        a.x * b.y - a.y * b.x
    };
}

Vec3 Vec3::rotate(const Vec3& rotatingVector, const Vec3& baseVector, const float radians)
{
    // Rodrigues' rotation formula:
    Vec3 k = cross(baseVector, rotatingVector).normalized();
    Vec3 rotatedVector = rotatingVector * cos(radians) + k * sin(radians) + k * (k * rotatingVector) * (1 - cos(radians));

    return rotatedVector;
}

void Vec3::print()
{
    printf("{%f, %f, %f}", x, y, z);
}

float Vec3::norm() const
{
    return sqrt(x * x + y * y + z * z);
}


/////////////////////////////////////////////////////////////////////////////////////////

Line::Line(Vec3 direction, Vec3 startingPoint) :
    dir{ direction },
    sPt{ startingPoint }
{
}

Vec3 Line::operator()(float lambda) const
{
    return sPt + lambda * dir;
}

/////////////////////////////////////////////////////////////////////////////////////////

Plane::Plane(Vec3 normal, Vec3 point) :
    n{ normal },
    pt{ point }
{
    d = -(n * pt);
}

/////////////////////////////////////////////////////////////////////////////////////////

Vec3 linePlaneIntersection(const Line& l, const Plane& p)
{
    Vec3 normal = p.n;
    Vec3 direction = l.dir;

    assert(normal * direction != 0.0f);

    float lambda = -(normal * l.sPt + p.d) / (normal * direction);

    return l(lambda);
}

Vec3 perpendicularVector(const Vec3& v)
{
    Vec3 ret;
    if (abs(v.y) < DELTA && abs(v.z) < DELTA) // y == 0 && z == 0
    {
        ret = Vec3::cross({ 0, 1, 0 }, v);
    }
    else if (abs(v.x) < DELTA && abs(v.z) < DELTA)
    {
        ret = Vec3::cross({ 1, 0, 0 }, v);
    }
    else
    {
        ret = Vec3::cross({ 1, 0, 1 }, v); // something random
    }

    return ret.normalized();
}

