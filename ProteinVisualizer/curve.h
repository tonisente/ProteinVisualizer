#pragma once
#include "mathLib.h"

#include <vector>
#include <DirectXMath.h>

class Curve
{
public:
    static Vec3 catmullRom(const float t, const float tension, const Vec3 p0, const Vec3 p1, const Vec3 p2, const Vec3 p3);
    static Vec3 catmullRomTangent(const float t, const float tension, const Vec3 p0, const Vec3 p1, const Vec3 p2, const Vec3 p3);

    Curve(const std::vector<Vec3> v, const float tension);
    ~Curve();

    /** Returns point for value t. */
    Vec3 operator()(float t) const;
    /** Returns curve tangent vector. */
    Vec3 operator[](float t) const;

private:
    static float catmullRomPointMatrix(const float t, const float a, const float x0, const float x1, const float x2, const float x3);
    static float catmullRomTangentMatrix(const float t, const float a, const float x0, const float x1, const float x2, const float x3);

private:
    const std::vector<Vec3>& m_points;
    const float m_tension;
};
