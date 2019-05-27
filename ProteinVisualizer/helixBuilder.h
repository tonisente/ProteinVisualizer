#pragma once

#include "mathLib.h"
#include "vertex.h"

#include <vector>

typedef unsigned int uint;

class HelixBuilder
{
public:
    HelixBuilder();
    ~HelixBuilder();

    float width = 0.5f;
    Vec3 color{ 1.0f, 1.0f, 0.5f };

    void buildHelix(const std::vector<Vec3>& points, const std::vector<Vec3>& tangentVectors, std::vector<Vertex>& vertices, std::vector<uint>& indices);
};
