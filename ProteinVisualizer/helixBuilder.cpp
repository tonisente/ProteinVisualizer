#include "helixBuilder.h"


HelixBuilder::HelixBuilder()
{
}


HelixBuilder::~HelixBuilder()
{
}


void HelixBuilder::buildHelix(const std::vector<Vec3>& points, const std::vector<Vec3>& normals, std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
    assert(points.size() >= 3);
    assert(points.size() == normals.size());

    uint n = points.size();

    vertices.clear();
    indices.clear();
    vertices.reserve(2 * n);

    for (int i = 0; i < n - 1; ++i)
    {
        Vec3 rightVec = Vec3::cross(normals[i], normals[i + 1]).normalized() * width;
        Vec3 basePoint = points[i];
        vertices.push_back(basePoint + rightVec);
        vertices.push_back(basePoint + rightVec);




    }
}


