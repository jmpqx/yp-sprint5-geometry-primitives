#include "general_defines.hpp"
#include "geometry.hpp"
#include "queries.hpp"
#include <gtest/gtest.h>

using geometry::Circle;
using geometry::Line;
using geometry::Point2D;
using geometry::Rectangle;
using geometry::Triangle;
using geometry::queries::DistanceBetweenShapes;
using geometry::queries::DistanceToPoint;
using geometry::queries::GetHeight;

struct QueryParam {
    geometry::Shape shape;
    Point2D point;
    double expected_distance;
};

class DistanceToPointTest : public ::testing::TestWithParam<QueryParam> {};

TEST_P(DistanceToPointTest, Distance) {
    const auto &param = GetParam();
    double d = DistanceToPoint(param.shape, param.point);
    EXPECT_NEAR(d, param.expected_distance, defs::epsilon);
}

INSTANTIATE_TEST_SUITE_P(
    DistanceCases, DistanceToPointTest,
    ::testing::Values(QueryParam{.shape = Line{{0, 0}, {1, 0}}, .point = {0, 1}, .expected_distance = 1.0},
                      QueryParam{.shape = Triangle{{0, 0}, {1, 0}, {0, 1}}, .point = {0, 0}, .expected_distance = 0.0},
                      QueryParam{.shape = Rectangle{{0, 0}, 2, 2}, .point = {1, 1}, .expected_distance = 1.0},
                      QueryParam{.shape = Circle{{0, 0}, 1}, .point = {2, 0}, .expected_distance = 1.0}));

TEST(QueriesTest, DistanceBetweenShapes) {
    geometry::Shape s1 = Line{{0, 0}, {1, 0}};
    geometry::Shape s2 = Line{{0, 2}, {1, 2}};
    auto d = DistanceBetweenShapes(s1, s2);
    ASSERT_TRUE(d.has_value());
    EXPECT_NEAR(*d, 2.0, defs::epsilon);
}

TEST(QueriesTest, GetHeight) {
    geometry::Shape s = Rectangle{{0, 0}, 2, 3};
    EXPECT_DOUBLE_EQ(GetHeight(s), 3.0);
}
