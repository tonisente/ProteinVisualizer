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


void TubeBuilder::buildWireframe(const std::vector<Vec3>& points, std::vector<Vertex>& vertices, std::vector<uint>& indices) const
{
    if (points.size() == 0) return;

    uint n = points.size();
    vertices.clear();
    indices.clear();
    vertices.reserve(sides * n);
    indices.reserve(sides * n);

    assert(n >= 2);

    Vec3 lowerPoint = points[0];
    Vec3 upperPoint = points[1];

    // generate base points
    std::vector<Vertex> prevPoints = baseTubePoints(lowerPoint, upperPoint);
    // put them into vertex buffer (still nothing to insert into index buffer)
    vertices.insert(vertices.end(), prevPoints.begin(), prevPoints.end());

    // create all other points and generate triangles (indices);
    for (int i = 0; i < n - 1; ++i)
    {
        {   // generate vertices
            lowerPoint = points[i];
            upperPoint = points[i + 1];

            Vec3 outVec = upperPoint - lowerPoint; // valid only for last tube piece.
            if (i < n - 2)
            {
                outVec = points[i + 2] - upperPoint;
            }

            std::vector<Vertex> nextPoints = nextTubePoints(prevPoints, lowerPoint, upperPoint, outVec);
            vertices.insert(vertices.end(), nextPoints.begin(), nextPoints.end());
            prevPoints = std::move(nextPoints);
        }

        {   //update indices (2 triangles per side)
            uint lowerBaseIndex = vertices.size() - 2 * sides;
            uint upperBaseIndex = vertices.size() - sides;
            for (int side = 0; side < sides; ++side)
            {
                // lower triangle
                indices.push_back(lowerBaseIndex + side);
                indices.push_back(lowerBaseIndex + (side + 1) % sides);
                indices.push_back(upperBaseIndex + side);

                // upper triangle
                indices.push_back(lowerBaseIndex + (side + 1) % sides);
                indices.push_back(upperBaseIndex + (side + 1) % sides);
                indices.push_back(upperBaseIndex + side);
            }
        }
    }
}


std::vector<Vertex> TubeBuilder::nextTubePoints(const std::vector<Vertex>& previousPoints, const Vec3& p0, const Vec3& p1, const Vec3& outVec) const
{
    Vec3 centralVec = p1 - p0;
    Vec3 radiusVec = perpendicularVector(centralVec);

    Plane upperPlane{ (centralVec + outVec) / 2, p1 };

    std::vector<Vertex> vertices;
    vertices.reserve(sides);
    
    // WTF?!
    for (const Vertex& basePoint : previousPoints)
    {
        Line sideLine{ centralVec, basePoint.position };
        Vec3 intersectionPoint = linePlaneIntersection(sideLine, upperPlane);
        Vertex v{ intersectionPoint, intersectionPoint - p1, color };
        vertices.emplace_back(v);
    }
        
    return vertices;
}


std::vector<Vertex> TubeBuilder::baseTubePoints(const Vec3& p0, const Vec3& p1) const
{
    Vec3 centralVec = p1 - p0;
    Vec3 radiusVec = perpendicularVector(centralVec);

    std::vector<Vertex> vertices;
    vertices.reserve(sides);

    for (int j = 0; j < sides; ++j)
    {
        float rotationDegree = ((float)j / (float)sides) * (2.0f * PI);
        Vec3 rotatedVec = Vec3::rotate(radiusVec, centralVec, rotationDegree);
        Vec3 point{ rotatedVec * thicknes + p0 };
        Vertex v{ point, (point - p0).normalized(), color };
        vertices.emplace_back(v);
    }

    return vertices;
}
