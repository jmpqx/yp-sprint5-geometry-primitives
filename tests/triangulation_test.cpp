#include "triangulation.hpp"
#include <expected>
#include <gtest/gtest.h>
#include <vector>

using geometry::Point2D;
using geometry::triangulation::DelaunayTriangulation;

TEST(TriangulationTest, NotEnoughPoints) {
    std::vector<Point2D> pts = {{0, 0}, {1, 1}};
    auto res = DelaunayTriangulation(pts);
    ASSERT_FALSE(res.has_value());
}

TEST(TriangulationTest, SimpleTriangle) {
    std::vector<Point2D> pts = {{0, 0}, {1, 0}, {0, 1}};
    auto res = DelaunayTriangulation(pts);
    ASSERT_TRUE(res.has_value());
    ASSERT_FALSE(res->empty());
}

TEST(TriangulationTest, SquarePoints) {
    std::vector<Point2D> pts = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    auto res = DelaunayTriangulation(pts);
    ASSERT_TRUE(res.has_value());
    // Должно быть 2 треугольника для квадрата
    EXPECT_EQ(res->size(), 2);
}
