#include "TubeBuilder.h"
#include <algorithm>


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


void TubeBuilder::buildCurvedWireframe(Curve& curve, const unsigned int noPoints, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) const
{
    // todo: reserve memory for vertices and indices
    const float sampleLength = 50.0f;// todo: put as func param

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

        std::vector<Vertex> tubePiece = tubeSample(p0 - prevPoint, p0, p1, nextPoint - p1);
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

void TubeBuilder::buildWireframe(const Chain& atoms, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) const
{
    vertices.reserve(sides * (atoms.size() + 1)); // every tube piece in wireframe needs its own vertices; 
    indices.reserve(sides * (atoms.size() + 1));

    for (int i = 0, n = atoms.size() - 1; i < n; ++i)
    {
        Vec3 p0{ atoms[i].xCoord    , atoms[i].yCoord    , atoms[i].zCoord };
        Vec3 p1{ atoms[i + 1].xCoord, atoms[i + 1].yCoord, atoms[i + 1].zCoord };

        Vec3 inVec, outVec;
        inVec = outVec = p1 - p0;

        if (i > 0)
        {   // update inVec to match previous one
            Vec3 pPrev{ atoms[i - 1].xCoord, atoms[i - 1].yCoord, atoms[i - 1].zCoord };
            inVec = p0 - pPrev;
        }
        if (i < n - 1)
        {   // update outVec to match next one
            Vec3 pNext{ atoms[i + 2].xCoord, atoms[i + 2].yCoord, atoms[i + 2].zCoord };
            outVec = pNext - p1;
        }

        // generate vertices
        std::vector<Vertex> tubePiece = tubeSample(inVec, p0, p1, outVec);

        {   // todo: extract this (from previous func too)
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
}




std::vector<Vertex> TubeBuilder::tubeSample(const Vec3 inVec, const Vec3 p0, const Vec3 p1, const Vec3 outVec) const
{
    Vec3 centralVec = p1 - p0;
    Vec3 radiusVec = perpendicularVector(centralVec);

    Plane plane1{ (inVec + centralVec) / 2, p0 };
    Plane plane2{ (centralVec + outVec) / 2, p1 };

    std::vector<Vertex> vertices;
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
        Vertex v{ lowerPoint, lowerPointNormal, color };
        vertices.push_back(v);

        // vertex on next plane
        Vec3 upperPoint = linePlaneIntersection(sideLine, plane2);
        Vec3 upperPointNormal = upperPoint - p1;
        v = { upperPoint, upperPointNormal, color };
        vertices.push_back(v);
    }

    return vertices;
}



