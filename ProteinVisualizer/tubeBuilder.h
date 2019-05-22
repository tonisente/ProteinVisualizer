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

    unsigned int sides = 20;
    unsigned int partsPerCurveSegment = 10;
    float thicknes = 0.2f;
    float tension = 1.0f; // 0.0f -> straight line ; 1.0f -> very curvy lines
    Vec3 color{ 1.0f, 1.0f, 0.5f };

    void buildCurvedWireframe(const std::vector<Vec3>& atoms, std::vector<Vertex>& vertices, std::vector<uint>& indices) const;
    void buildWireframe(const std::vector<Vec3>& atoms, std::vector<Vertex>& vertices, std::vector<uint>& indices) const;

private:
    std::vector<Vertex> tubeSample(const Vec3 tPrev, const Vec3 t0, const Vec3 t1, const Vec3 tNext) const;
    
};

