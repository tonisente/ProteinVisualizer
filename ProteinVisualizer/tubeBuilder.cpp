#include "TubeBuilder.h"
#include <algorithm>


TubeBuilder::TubeBuilder()
{
}

TubeBuilder::TubeBuilder(uint noSides, float thicknes):
    sides{ noSides },
    thicknes{ thicknes }
{
}


TubeBuilder::~TubeBuilder()
{
}


void TubeBuilder::buildCurvedWireframe(const std::vector<Vec3>& atoms, std::vector<Vertex>& vertices, std::vector<uint>& indices) const
{
    Vec3 p0, p1, p2, p3;
    uint segments = atoms.size() - 1;
    for (uint i = 0; i < segments; ++i)
    {
        p0 = (i == 0) ? atoms[1].opposite(atoms[0]) : atoms[i - 1];
        p1 = atoms[i];
        p2 = atoms[i + 1];
        p3 = (i == segments - 1) ? atoms[atoms.size() - 2].opposite(atoms[atoms.size() - 1]) : atoms[i + 2];
        
        for (int j = 0; j < partsPerCurveSegment; ++j)
        {
            float lowerT = (float)j / float(partsPerCurveSegment);
            float upperT = (float)(j + 1) / float(partsPerCurveSegment);
            Vec3 lowerPoint = Curve::catmullRom(lowerT, tension, p0, p1, p2, p3);
            Vec3 upperPoint = Curve::catmullRom(upperT, tension, p0, p1, p2, p3);
            Vec3 inVec = Curve::catumullRomTangent(lowerT, tension, p0, p1, p2, p3);
            Vec3 outVec = Curve::catumullRomTangent(upperT, tension, p0, p1, p2, p3);

            generateAndInsert(inVec, lowerPoint, upperPoint, outVec, vertices, indices);
        }
    }
}

void TubeBuilder::buildWireframe(const std::vector<Vec3>& atoms, std::vector<Vertex>& vertices, std::vector<uint>& indices) const
{
    vertices.reserve(sides * (atoms.size() + 1)); // every tube piece in wireframe needs its own vertices; 
    indices.reserve(sides * (atoms.size() + 1));

    for (int i = 0, n = atoms.size() - 1; i < n; ++i)
    {
        Vec3 lowerPoint = atoms[i];
        Vec3 upperPoint = atoms[i + 1];

        Vec3 inVec, outVec;
        inVec = outVec = upperPoint - lowerPoint;

        if (i > 0)
        {   // update inVec to match previous one
            Vec3 pPrev = atoms[i - 1];
            inVec = lowerPoint - pPrev;
        }
        if (i < n - 1)
        {   // update outVec to match next one
            Vec3 pNext = atoms[i + 2];
            outVec = pNext - upperPoint;
        }

        generateAndInsert(inVec, lowerPoint, upperPoint, outVec, vertices, indices);
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


void TubeBuilder::generateAndInsert(const Vec3 inVec, const Vec3 lowerPoint, const Vec3 upperPoint, const Vec3 outVec, std::vector<Vertex>& vertices, std::vector<uint>& indices) const
{
    std::vector<Vertex> tubePieces = tubeSample(inVec, lowerPoint, upperPoint, outVec);

    {   // save base index
        uint baseIdx = vertices.size();

        for (int j = 0, tubePieceSize = tubePieces.size(); j < tubePieceSize; ++j)
        {   // todo: use memcpy or push directly in vertices vector?
            vertices.push_back(tubePieces[j]);
        }

        // update indices (2 triangles per side)
        uint mod = 2 * sides; // needed because first tube sample vertices are also used for last triangle
        for (int k = 0; k < sides; ++k)
        {
            uint vIndex = k * 2;

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
