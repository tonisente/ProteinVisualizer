#pragma once
#include "mathLib.h"

struct Vertex
{
    Vec3 position;
    Vec3 normal;
    Vec3 color; // alpha of color is always 1

    Vertex(const Vec3& pos, const Vec3& nor, const Vec3& col):
        position{ pos },
        normal{ nor },
        color{ col }
    {
    }

};

