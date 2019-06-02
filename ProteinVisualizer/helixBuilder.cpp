#include "helixBuilder.h"
#include "curve.h"


HelixBuilder::HelixBuilder()
{
}


HelixBuilder::~HelixBuilder()   
{
}


void HelixBuilder::buildHelix(const std::vector<Vec3>& points, const std::vector<Vec3>& tangentVectors, uint partsPerCurveSegment, std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
    assert(points.size() >= 3);
    assert(points.size() == tangentVectors.size());

    uint n = points.size();

    vertices.clear();
    indices.clear();
    vertices.reserve(2 * n);
    indices.reserve(6 * (n - 1));

    {   // generate vertices
        for (int i = 1; i < n - 1; i++)
        {
            //Vec3 tangent = (points[i + 1] - points[i - 1]).normalized();
            Vec3 tangent = tangentVectors[i].normalized();
            Vec3 helpVec = (points[i] - points[i - 1]);
            Vec3 up = (Vec3::cross(tangent, helpVec)).normalized();
            Vec3 right = (Vec3::cross(up, tangent)).normalized();

            {
                Vec3 downLeft = points[i] - up * height - right * width;
                Vec3 downRight = points[i] - up * height + right * width;
                Vec3 upLeft = points[i] + up * height - right * width;
                Vec3 upRight = points[i] + up * height + right * width;

                vertices.push_back(Vertex(downLeft, right * (-1), color));
                vertices.push_back(Vertex(downRight, right, color));
                vertices.push_back(Vertex(upLeft, right * (-1), color));
                vertices.push_back(Vertex(upRight, right, color));
            }
        }
    }
    {   // generate indices
        for (int i = 0; i < n - 3; ++i)
        {
            int base = i * 4;

            indices.push_back(base);
            indices.push_back(base + 4 + 2);
            indices.push_back(base + 4);

            indices.push_back(base);
            indices.push_back(base + 2);
            indices.push_back(base + 4 + 2);


            indices.push_back(base + 2);
            indices.push_back(base + 4 + 3);
            indices.push_back(base + 3);

            indices.push_back(base + 2);
            indices.push_back(base + 4 + 2);
            indices.push_back(base + 4 + 3);


            indices.push_back(base + 1);
            indices.push_back(base + 3);
            indices.push_back(base + 4 + 3);

            indices.push_back(base + 1);
            indices.push_back(base + 4 + 3);
            indices.push_back(base + 4 + 1);


            indices.push_back(base);
            indices.push_back(base + 4 + 1);
            indices.push_back(base + 1);

            indices.push_back(base);
            indices.push_back(base + 4);
            indices.push_back(base + 4 + 1);
        }

    }
}



struct CoordSystem
{
    Vec3 x;
    Vec3 y;
    Vec3 z;

    CoordSystem()
    {}

    CoordSystem(Vec3 xx, Vec3 yy, Vec3 zz):
        x{ xx.normalized() },
        y{ yy.normalized() },
        z{ zz.normalized() }
    {
    }

    void initFromPoints(Vec3 p0, Vec3 p1, Vec3 p2)
    {
        Vec3 tangent = p2 - p0;
        Vec3 helpVec = p1 - p0;
        Vec3 binormal = Vec3::cross(tangent, helpVec);
        Vec3 normal = Vec3::cross(binormal, tangent);

        x = normal.normalized();
        y = binormal.normalized();
        z = tangent.normalized();
    }
};

CoordSystem coordinateSlepr(float t, CoordSystem s1, CoordSystem s2, Vec3 tangent)
{
    return
    {
        s1.x * (1 - t) + s2.x * t,
        s1.y * (1 - t) + s2.y * t,
        tangent
    };
}

void HelixBuilder::buildHelix_v2(const std::vector<Vec3>& basePoints, std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
    uint n = basePoints.size();
    int parts = 10;

    for (int i = 1; i < n - 2; ++i)
    {
        CoordSystem s1, s2;
        Vec3 p0 = basePoints[i - 1];
        Vec3 p1 = basePoints[i];
        Vec3 p2 = basePoints[i + 1];
        Vec3 p3 = basePoints[i + 2];

        s1.initFromPoints(p0, p1, p2);
        s2.initFromPoints(p1, p2, p3);

        std::vector<Vec3> points;
        std::vector<Vec3> tangents;

        for (int j = 0; j < parts; ++j)
        {
            float t = float(j) / float(parts);
            Vec3 curvePoint = Curve::catmullRom(t, 1.0f, p0, p1, p2, p3);
            Vec3 curveTangent = Curve::catmullRomTangent(t, 1.0f, p0, p1, p2, p3);

            CoordSystem s = coordinateSlepr(t, s1, s2, curveTangent);

            Vec3 up = s.y;
            Vec3 right = s.x;

            {
                Vec3 downLeft = curvePoint - up * height - right * width;
                Vec3 downRight = curvePoint - up * height + right * width;
                Vec3 upLeft = curvePoint + up * height - right * width;
                Vec3 upRight = curvePoint + up * height + right * width;

                vertices.push_back(Vertex(downLeft, right * (-1), color));
                vertices.push_back(Vertex(downRight, right, color));
                vertices.push_back(Vertex(upLeft, right * (-1), color));
                vertices.push_back(Vertex(upRight, right, color));
            }
        }
    }


    {   // generate indices
        for (int i = 0; i < parts * n - 1; ++i)
        {
            int base = i * 4;

            indices.push_back(base);
            indices.push_back(base + 4 + 2);
            indices.push_back(base + 4);

            indices.push_back(base);
            indices.push_back(base + 2);
            indices.push_back(base + 4 + 2);


            indices.push_back(base + 2);
            indices.push_back(base + 4 + 3);
            indices.push_back(base + 3);

            indices.push_back(base + 2);
            indices.push_back(base + 4 + 2);
            indices.push_back(base + 4 + 3);


            indices.push_back(base + 1);
            indices.push_back(base + 3);
            indices.push_back(base + 4 + 3);

            indices.push_back(base + 1);
            indices.push_back(base + 4 + 3);
            indices.push_back(base + 4 + 1);


            indices.push_back(base);
            indices.push_back(base + 4 + 1);
            indices.push_back(base + 1);

            indices.push_back(base);
            indices.push_back(base + 4);
            indices.push_back(base + 4 + 1);
        }
    }
}





//    {   // generate veritces
//        Vec3 rightVec;
//        for (int i = 0; i < n; ++i)
//        {
//            // since last cross product can't be calculated, use the previous one that has been calculated. 
//            if (i < n - 2)
//            {   
//                rightVec = Vec3::cross(tangentVectors[i], tangentVectors[i + 2]);
//            }
//
//            assert(rightVec.x != 0.0f && rightVec.y != 0.0f && rightVec.z != 0.0f);
//
//            rightVec = rightVec.normalized();
//            Vec3 normal = Vec3::cross(tangentVectors[i], rightVec);
//            rightVec = rightVec * width;
//            Vec3 basePoint = points[i];
//
//            vertices.push_back({ basePoint + rightVec, normal, color });
//            vertices.push_back({ basePoint - rightVec, normal, color });
//        }
//    }
//
//    // update indices
//    for (uint i = 0; i < n - 1; ++i)
//    {
//        uint base = i * 2;
//
//        // 1st triangle
//        indices.push_back(base);
//        indices.push_back(base + 1);
//        indices.push_back(base + 2);
//
//        // 2nd triangle
//        indices.push_back(base + 1);
//        indices.push_back(base + 2);
//        indices.push_back(base + 3);
//    }
//}

void HelixBuilder::buildRibbon(const Vec3 pBefore, const std::vector<Vec3>& points, const Vec3 pAfter, std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
    uint n = points.size();
    int parts = 10;

    for (int i = 0; i < n - 1; ++i)
    {
        CoordSystem s1, s2;
        Vec3 p0 = i == 0 ? pBefore : points[i - 1];
        Vec3 p1 = points[i];
        Vec3 p2 = points[i + 1];
        Vec3 p3 = i == n - 2 ? pAfter : points[i + 2];

        s1.initFromPoints(p0, p1, p2);
        s2.initFromPoints(p1, p2, p3);

        std::vector<Vec3> points;
        std::vector<Vec3> tangents;

        for (int j = 0; j < parts; ++j)
        {
            float t = float(j) / float(parts);
            Vec3 curvePoint = Curve::catmullRom(t, 1.0f, p0, p1, p2, p3);
            Vec3 curveTangent = Curve::catmullRomTangent(t, 1.0f, p0, p1, p2, p3);

            CoordSystem s = coordinateSlepr(t, s1, s2, curveTangent);

            Vec3 up = s.y;
            Vec3 right = s.x;

            {
                Vec3 downLeft = curvePoint - up * height - right * width;
                Vec3 downRight = curvePoint - up * height + right * width;
                Vec3 upLeft = curvePoint + up * height - right * width;
                Vec3 upRight = curvePoint + up * height + right * width;

                vertices.push_back(Vertex(downLeft, right * (-1), color));
                vertices.push_back(Vertex(downRight, right, color));
                vertices.push_back(Vertex(upLeft, right * (-1), color));
                vertices.push_back(Vertex(upRight, right, color));
            }
        }
    }


    {   // generate indices
        for (int i = 0; i < (parts + 1) * points.size(); ++i)
        {
            int base = i * 4;
             
            indices.push_back(base);
            indices.push_back(base + 4);
            indices.push_back(base + 4 + 2);

            indices.push_back(base);
            indices.push_back(base + 4 + 2);
            indices.push_back(base + 2);


            indices.push_back(base + 2);
            indices.push_back(base + 4 + 3);
            indices.push_back(base + 3);

            indices.push_back(base + 2);
            indices.push_back(base + 4 + 2);
            indices.push_back(base + 4 + 3);


            indices.push_back(base + 1);
            indices.push_back(base + 3);
            indices.push_back(base + 4 + 3);

            indices.push_back(base + 1);
            indices.push_back(base + 4 + 3);
            indices.push_back(base + 4 + 1);


            indices.push_back(base);
            indices.push_back(base + 1);
            indices.push_back(base + 4 + 1);

            indices.push_back(base);
            indices.push_back(base + 4 + 1);
            indices.push_back(base + 4);
        }
    }
}
