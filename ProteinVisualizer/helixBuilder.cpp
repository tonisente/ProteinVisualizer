#include "helixBuilder.h"


HelixBuilder::HelixBuilder()
{
}


HelixBuilder::~HelixBuilder()   
{
}


void HelixBuilder::buildHelix(const std::vector<Vec3>& points, const std::vector<Vec3>& tangentVectors, std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
    assert(points.size() >= 3);
    assert(points.size() == tangentVectors.size());

    uint n = points.size();

    vertices.clear();
    indices.clear();
    vertices.reserve(2 * n);
    indices.reserve(6 * (n - 1)); 

    {   // generate veritces
        Vec3 rightVec;
        for (int i = 0; i < n; ++i)
        {
            // since last cross product can't be calculated, use the previous one that has been calculated. 
            if (i <= n - 2)
            {   
                rightVec = Vec3::cross(tangentVectors[i], tangentVectors[i + 1]);
            }

            assert(rightVec.x != 0.0f && rightVec.y != 0.0f && rightVec.z != 0.0f);

            rightVec = rightVec.normalized();
            Vec3 normal = Vec3::cross(tangentVectors[i], rightVec);
            rightVec = rightVec * width;
            Vec3 basePoint = points[i];

            vertices.push_back({ basePoint + rightVec, normal, color });
            vertices.push_back({ basePoint - rightVec, normal, color });
        }
    }

    // update indices
    for (uint i = 0; i < n - 1; ++i)
    {
        uint base = i * 2;

        // 1st triangle
        indices.push_back(base);
        indices.push_back(base + 1);
        indices.push_back(base + 2);

        // 2nd triangle
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base + 3);
    }
}


