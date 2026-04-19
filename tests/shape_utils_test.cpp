#include "shape_utils.hpp"
#include <gtest/gtest.h>

#include "geometry.hpp"
#include "shape_utils.hpp"
#include <gtest/gtest.h>

using geometry::utils::FindAllCollisions;
using geometry::utils::FindHighestShape;
using geometry::utils::ParseShapes;

TEST(ShapeUtilsTest, ParseShapes) {
    auto shapes = ParseShapes("circle 0 0 1; line 0 0 1 1; triangle 0 0 1 0 0 1");
    ASSERT_EQ(shapes.size(), 3);
}

TEST(ShapeUtilsTest, FindAllCollisions) {
    std::vector<geometry::Shape> shapes = ParseShapes("rectangle 0 0 2 2; rectangle 1 1 2 2; rectangle 5 5 1 1");
    auto collisions = FindAllCollisions(shapes);
    ASSERT_EQ(collisions.size(), 1);
}

TEST(ShapeUtilsTest, FindHighestShape) {
    std::vector<geometry::Shape> shapes = ParseShapes("rectangle 0 0 2 2; rectangle 1 5 2 2; rectangle 5 1 1 1");
    auto idx = FindHighestShape(shapes);
    ASSERT_TRUE(idx.has_value());
    EXPECT_EQ(*idx, 1);
}
