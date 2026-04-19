#include "intersections.hpp"
#include <gtest/gtest.h>

#include "intersections.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

using geometry::Circle;
using geometry::Line;
using geometry::Point2D;
using geometry::intersections::GetIntersectPoint;

TEST(IntersectionsTest, LineLineIntersection) {
    Line l1{{0, 0}, {2, 2}};
    Line l2{{0, 2}, {2, 0}};
    auto res = GetIntersectPoint(l1, l2);
    ASSERT_TRUE(res.has_value());
    EXPECT_NEAR(res->x, 1.0, 1e-9);
    EXPECT_NEAR(res->y, 1.0, 1e-9);
}

TEST(IntersectionsTest, LineCircleIntersection) {
    Line l{{-1, 0}, {1, 0}};
    Circle c{{0, 0}, 1};
    auto res = GetIntersectPoint(l, c);
    ASSERT_TRUE(res.has_value());
    EXPECT_NEAR(res->y, 0.0, 1e-9);
}

TEST(IntersectionsTest, CircleCircleIntersection) {
    Circle c1{{0, 0}, 1};
    Circle c2{{2, 0}, 1};
    auto res = GetIntersectPoint(c1, c2);
    ASSERT_TRUE(res.has_value());
    EXPECT_NEAR(res->y, 0.0, 1e-9);
}

TEST(IntersectionsTest, UnsupportedTypesThrows) {
    struct Dummy {};
    geometry::Shape s1 = Line{{0, 0}, {1, 1}};
    geometry::Shape s2 = geometry::Triangle{{0, 0}, {1, 0}, {0, 1}};
    EXPECT_THROW(GetIntersectPoint(s1, s2), std::logic_error);
}
