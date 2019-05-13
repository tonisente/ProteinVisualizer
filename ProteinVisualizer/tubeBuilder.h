#pragma once
#include "curve.h"
#include "MathLib.h"
#include <vector>

class TubeBuilder
{
public:
    TubeBuilder();
    TubeBuilder(unsigned int noSides, float thicknes);
    ~TubeBuilder();

    unsigned int sides = 11;
    float thicknes = 0.2f;

    void buildTube(Curve& curve, const unsigned int noPoints, std::vector<std::pair<Vec3, Vec3>>& vertices, std::vector<unsigned int>& indices) const;

private:
    std::vector<std::pair<Vec3, Vec3>> tubeSample(const Vec3 tPrev, const Vec3 t0, const Vec3 t1, const Vec3 tNext) const;
};

