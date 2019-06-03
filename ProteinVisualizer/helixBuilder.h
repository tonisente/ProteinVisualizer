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

    /** Parts per 1 curve segment between two base points. */
    uint ppSegment = 6;
    float width = 0.1f;
    float height = 1.0f;
    Vec3 color{ 1.0f, 0.3f, 0.3f };

    //void buildHelix(const std::vector<Vec3>& points, const std::vector<Vec3>& tangentVectors, uint partsPerCurveSegment, std::vector<Vertex>& vertices, std::vector<uint>& indices);
    void buildRibbon(const Vec3 p0, const std::vector<Vec3>& points, const Vec3 pn, std::vector<Vertex>& vertices, std::vector<uint>& indices, bool correctSharpTurns);
    //void buildHelix_v2(const std::vector<Vec3>& points, std::vector<Vertex>& vertices, std::vector<uint>& indices);
};
