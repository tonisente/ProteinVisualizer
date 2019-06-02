#pragma once

#include <cmath>
#include <cassert>
#include <cstdio>
#include <DirectXMath.h>

constexpr double PI = 3.14159265358979323846;
constexpr double DELTA = 1e-7;

struct Vec3
{
    float x;
    float y;
    float z;

    Vec3();
    Vec3(const float a, const float b, const float c);

    Vec3 operator=(const Vec3& other);
    friend float operator*(const Vec3& a, const Vec3& b);
    friend Vec3 operator*(const Vec3& v, const float lambda);
    friend Vec3 operator*(const float lambda, const Vec3& v);
    friend Vec3 operator/(const Vec3& v, const float lambda);
    friend Vec3 operator/(const float lambda, const Vec3&v);
    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;

    Vec3 normalized() const;
    static Vec3 cross(const Vec3& a, const Vec3& b);
    static Vec3 rotate(const Vec3& rotatingVector, const Vec3& baseVector, const float radians);
    void print();
    float norm() const;
    DirectX::XMFLOAT3 toXMFLOAT3();
};


struct Line
{
    Vec3 dir;
    Vec3 sPt;

    Line(Vec3 direction, Vec3 startingPoint);
    Vec3 operator()(float lambda) const;
};


struct Plane
{
    Vec3 n;
    Vec3 pt;
    float d;

    Plane(Vec3 normal, Vec3 point);
};


Vec3 linePlaneIntersection(const Line& l, const Plane& p);

Vec3 perpendicularVector(const Vec3& v);
