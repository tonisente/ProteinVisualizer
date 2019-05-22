#pragma once
#include <gtest/gtest.h> // TEST

//#include "../ProteinVisualizer/mathLib.h"
#include "../ProteinVisualizer/curve.h"

TEST(Curve, StraightLine) {
    //Vec3 p0{ 0, 0, 0 };
    //Vec3 p1{ 1, 0, 0 };
    //Vec3 p2{ 2, 0, 0 };
    //Vec3 p3{ 3, 0, 0 };
    //const float a = 0.5f;
    //float t = 0.0f;
    //Vec3 result;

    //result = Curve::catmullRom(p0, p1, p2, p3, t, a);

    //Vec3 t;


    EXPECT_NEAR(0.0001, 0.0001, 0.01f);
}