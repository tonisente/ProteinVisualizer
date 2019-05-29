#pragma once
#include <vector>

#include "curve.h"
#include "mathLib.h"
#include "proteinData.h"
#include "vertex.h"

typedef unsigned int uint;

class TubeBuilder
{
public:
    TubeBuilder();
    TubeBuilder(unsigned int noSides, float thicknes);
    ~TubeBuilder();

    unsigned int sides = 16;
    float thicknes = 0.2f;
    Vec3 color{ 1.0f, 1.0f, 0.5f };

    void buildWireframe(const std::vector<Vec3>& atoms, std::vector<Vertex>& vertices, std::vector<uint>& indices) const;

private:
    std::vector<Vertex> nextTubePoints(const std::vector<Vertex>& previousPoints, const Vec3& p0, const Vec3& p1, const Vec3& outVec = { 0.0f, 0.0f, 0.0f }) const;
    std::vector<Vertex> baseTubePoints(const Vec3& p0, const Vec3& p1) const;
};

