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

    float width = 0.1f;
    float height = 1.0f;
    Vec3 color{ 1.0f, 0.6f, 0.6f };

    void buildHelix(const std::vector<Vec3>& points, const std::vector<Vec3>& tangentVectors, uint partsPerCurveSegment, std::vector<Vertex>& vertices, std::vector<uint>& indices);
    void buildHelix_v2(const std::vector<Vec3>& points, std::vector<Vertex>& vertices, std::vector<uint>& indices);
};
