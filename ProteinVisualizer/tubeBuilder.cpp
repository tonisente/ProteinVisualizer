#include "TubeBuilder.h"


TubeBuilder::TubeBuilder()
{
}

TubeBuilder::TubeBuilder(unsigned int noSides, float thicknes):
    sides{ noSides },
    thicknes{ thicknes }
{
}


TubeBuilder::~TubeBuilder()
{
}


void TubeBuilder::buildTube(Curve& curve, const unsigned int noPoints, std::vector<std::pair<Vec3, Vec3>>& vertices, std::vector<unsigned int>& indices) const
{
    const float sampleLength = 100.0f;

    Vec3 prevPoint;
    Vec3 p0 = curve(0.0f);
    Vec3 p1 = curve(1.0f / (float)noPoints * sampleLength);
    Vec3 nextPoint = curve(2.0f / (float)noPoints * sampleLength);

    for (int i = 3; i < noPoints - 1; ++i)
    {
        prevPoint = p0;
        p0 = p1;
        p1 = nextPoint;
        nextPoint = curve((float)i / (float)noPoints * sampleLength);

        std::vector<std::pair<Vec3, Vec3>> tubePiece = tubeSample(prevPoint, p0, p1, nextPoint);
        unsigned int baseIdx = vertices.size();

        for (int j = 0, tubePieceSize = tubePiece.size(); j < tubePieceSize; ++j)
        {   // todo: use memcpy or push directly in vertices vector?
            vertices.push_back(tubePiece[j]);
        }

        // update indices (2 triangles per side)
        unsigned int mod = 2 * sides; // needed because first tube sample vertices are also used for last triangle
        for (int k = 0; k < sides; ++k)
        {
            unsigned int vIndex = k * 2;

            // "lower" triangle
            indices.push_back(vIndex + baseIdx);
            indices.push_back(vIndex + 1 + baseIdx);
            indices.push_back((vIndex + 2) % mod + baseIdx);

            // "upper" triangle
            indices.push_back(vIndex + 1 + baseIdx);
            indices.push_back((vIndex + 3) % mod + baseIdx);
            indices.push_back((vIndex + 2) % mod + baseIdx);
        }
    }
}


std::vector<std::pair<Vec3, Vec3>> TubeBuilder::tubeSample(const Vec3 prevPoint, const Vec3 p0, const Vec3 p1, const Vec3 nextPoint) const
{
    Vec3 inVec = p0 - prevPoint;
    Vec3 centralVec = p1 - p0;
    Vec3 outVec = nextPoint - p1;

    Vec3 radiusVec = perpendicularVector(centralVec);

    Plane plane1{ (inVec + centralVec) / 2, p0 };
    Plane plane2{ (centralVec + outVec) / 2, p1 };

    std::vector<std::pair<Vec3, Vec3>> vertices;
    vertices.reserve(sides * 2);

    for (int j = 0; j < sides; ++j)
    {
        float rotationDegree = ((float)j / (float)sides) * (2.0f * PI);
        auto rotatedVec = Vec3::rotate(radiusVec, centralVec, rotationDegree);
        Vec3 sideBasePoint = rotatedVec * thicknes + p0;
        Line sideLine{ centralVec, sideBasePoint };

        // vertex on previous plane
        Vec3 lowerPoint = linePlaneIntersection(sideLine, plane1);
        Vec3 lowerPointNormal = lowerPoint - p0;
        vertices.push_back(std::make_pair(lowerPoint, lowerPointNormal));

        // vertex on next plane
        Vec3 upperPoint = linePlaneIntersection(sideLine, plane2);
        Vec3 upperPointNormal = upperPoint - p1;
        vertices.push_back(std::make_pair(upperPoint, upperPointNormal));
    }

    return vertices;
}



