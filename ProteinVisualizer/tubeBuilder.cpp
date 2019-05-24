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
    std::vector<Vec3> points;
    uint n = atoms.size();
    points.reserve(n * partsPerCurveSegment);

    {   // generate all points for curve
        Vec3 p0, p1, p2, p3;
        for (uint i = 0; i < n - 1; ++i)
        {
            p0 = (i == 0) ? atoms[1].opposite(atoms[0]) : atoms[i - 1];
            p1 = atoms[i];
            p2 = atoms[i + 1];
            p3 = (i == n - 2) ? atoms[n - 2].opposite(atoms[n - 1]) : atoms[i + 2];

            for (int j = 0; j < partsPerCurveSegment; ++j)
            {
                float t = (float)j / float(partsPerCurveSegment);
                Vec3 point = Curve::catmullRom(t, tension, p0, p1, p2, p3);

                points.push_back(point);
            }
        }

        // add last point
        points.push_back(atoms[n - 1]);
    }

    buildWireframe(points, vertices, indices);
}

void TubeBuilder::buildWireframe(const std::vector<Vec3>& points, std::vector<Vertex>& vertices, std::vector<uint>& indices) const
{
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
