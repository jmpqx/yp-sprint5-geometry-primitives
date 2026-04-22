#include "general_defines.hpp"
#include "geometry.hpp"
#include <gtest/gtest.h>

using geometry::BoundingBox;
using geometry::Line;
using geometry::Point2D;
using geometry::Rectangle;
using geometry::RegularPolygon;
using geometry::Triangle;

TEST(Point2DTest, OperatorsAndDistance) {
    Point2D a{1, 2}, b{4, 6};
    auto res = Point2D{5, 8};
    EXPECT_EQ(a + b, res);
    res = Point2D{3, 4};
    EXPECT_EQ(b - a, res);
    EXPECT_DOUBLE_EQ(a.Dot(b), 1 * 4 + 2 * 6);
    EXPECT_DOUBLE_EQ(a.Cross(b), 1 * 6 - 2 * 4);
    EXPECT_DOUBLE_EQ(a.Length(), std::sqrt(1 * 1 + 2 * 2));
    EXPECT_DOUBLE_EQ(a.DistanceTo(b), std::sqrt(9 + 16));
    EXPECT_NEAR(a.Normalize().Length(), 1.0, defs::epsilon);
}

TEST(LineTest, Properties) {
    Line l{{0, 0}, {3, 4}};
    EXPECT_DOUBLE_EQ(l.Length(), 5.0);
    EXPECT_EQ(l.BoundBox().min_x, 0);
    EXPECT_EQ(l.BoundBox().max_y, 4);
    auto verts = l.Vertices();
    auto res = Point2D{0, 0};
    EXPECT_EQ(verts[0], res);
    res = Point2D{3, 4};
    EXPECT_EQ(verts[1], res);
}

TEST(TriangleTest, AreaAndVertices) {
    Triangle t{{0, 0}, {4, 0}, {0, 3}};
    EXPECT_DOUBLE_EQ(t.Area(), 6.0);
    auto verts = t.Vertices();
    auto res = Point2D{0, 0};
    EXPECT_EQ(verts[0], res);
    res = Point2D{4, 0};
    EXPECT_EQ(verts[1], res);
    res = Point2D{0, 3};
    EXPECT_EQ(verts[2], res);
}

TEST(RectangleTest, Properties) {
    Rectangle r{{1, 2}, 3, 4};
    auto res = Point2D{4, 6};
    EXPECT_EQ(r.TopRight(), res);
    EXPECT_EQ(r.BoundBox().min_x, 1);
    EXPECT_EQ(r.BoundBox().max_y, 6);
    auto verts = r.Vertices();
    res = Point2D{1, 2};
    EXPECT_EQ(verts[0], res);
    res = Point2D{4, 6};
    EXPECT_EQ(verts[2], res);
}

TEST(RegularPolygonTest, Vertices) {
    RegularPolygon p{{0, 0}, 1, 4};
    auto verts = p.Vertices();
    EXPECT_EQ(verts.size(), 4);
    EXPECT_NEAR(verts[0].x, 1.0, defs::epsilon);
    EXPECT_NEAR(verts[1].y, 1.0, defs::epsilon);
}

TEST(BoundingBoxTest, Overlaps) {
    BoundingBox a{0, 0, 2, 2}, b{1, 1, 3, 3}, c{3, 3, 4, 4};
    EXPECT_TRUE(a.Overlaps(b));
    EXPECT_FALSE(a.Overlaps(c));
}
