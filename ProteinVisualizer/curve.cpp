#include "curve.h"



Curve::Curve()
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

