#pragma once
#include <vector>

#include "curve.h"
#include "mathLib.h"
#include "proteinData.h"

class TubeBuilder
{
public:
    TubeBuilder();
    TubeBuilder(unsigned int noSides, float thicknes);
    ~TubeBuilder();

    unsigned int sides = 20;
    float thicknes = 0.5f;

    void buildCurvedTube(Curve& curve, const unsigned int noPoints, std::vector<std::pair<Vec3, Vec3>>& vertices, std::vector<unsigned int>& indices) const;
    void buildWireframe(const ProteinData& data, std::vector<std::pair<Vec3, Vec3>>& vertices, std::vector<unsigned int>& indices) const;

private:
    std::vector<std::pair<Vec3, Vec3>> tubeSample(const Vec3 tPrev, const Vec3 t0, const Vec3 t1, const Vec3 tNext) const;
};

