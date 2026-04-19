#include "visualization.hpp"
#include <gtest/gtest.h>

#include "geometry.hpp"
#include "triangulation.hpp"
#include "visualization.hpp"
#include <gtest/gtest.h>

using geometry::Point2D;
using geometry::Shape;
using geometry::triangulation::DelaunayTriangle;

// TEST(VisualizationTest, DrawShapesSmoke) {
//     std::vector<Shape> shapes;
//     shapes.emplace_back(geometry::Line{{0, 0}, {1, 1}});
//     shapes.emplace_back(geometry::Circle{{0, 0}, 1});
//     EXPECT_NO_THROW(geometry::visualization::Draw(shapes));
// }

// TEST(VisualizationTest, DrawTrianglesSmoke) {
//     std::vector<DelaunayTriangle> triangles;
//     triangles.emplace_back(Point2D{0, 0}, Point2D{1, 0}, Point2D{0, 1});
//     EXPECT_NO_THROW(geometry::visualization::Draw(triangles));
// }
