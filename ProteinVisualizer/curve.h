#pragma once
#include <DirectXMath.h>

#include "mathLib.h"

class Curve
{
public:
    Curve();
    ~Curve();

    Vec3 operator()(float t) const;
};

