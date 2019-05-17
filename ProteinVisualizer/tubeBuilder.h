#pragma once
#include <vector>

#include "curve.h"
#include "mathLib.h"
#include "proteinData.h"
#include "vertex.h"

class TubeBuilder
{
public:
    TubeBuilder();
    TubeBuilder(unsigned int noSides, float thicknes);
    ~TubeBuilder();

    unsigned int sides = 20;
    float thicknes = 0.2f;
    Vec3 color{ 1.0f, 1.0f, 0.5f };

    void buildCurvedWireframe(Curve& curve, const unsigned int noPoints, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) const;
    void buildWireframe(const Chain& atoms, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) const;

private:
    std::vector<Vertex> tubeSample(const Vec3 tPrev, const Vec3 t0, const Vec3 t1, const Vec3 tNext) const;
    
};

