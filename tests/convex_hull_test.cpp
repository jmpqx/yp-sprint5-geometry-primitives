#include "convex_hull.hpp"
#include <gtest/gtest.h>

#include "convex_hull.hpp"
#include <expected>
#include <gtest/gtest.h>
#include <vector>

using geometry::Point2D;
using geometry::convex_hull::GrahamScan;

struct ConvexHullParam {
    std::vector<Point2D> input;
    std::vector<Point2D> expected;
    bool expect_success;
};

class ConvexHullTest : public ::testing::TestWithParam<ConvexHullParam> {};

TEST_P(ConvexHullTest, GrahamScanTest) {
    const auto &param = GetParam();
    auto result = GrahamScan(const_cast<decltype(param.input) &>(param.input));

    if (param.expect_success) {
        ASSERT_TRUE(result.has_value()) << "Expected success, got error: " << result.error();
        ASSERT_EQ(result->size(), param.expected.size());

        for (const auto &pt : param.expected) {
            ASSERT_NE(std::find(result->begin(), result->end(), pt), result->end())
                << "Point " << pt.x << "," << pt.y << " not found";
        }
    } else {
        ASSERT_FALSE(result.has_value());
    }
}

INSTANTIATE_TEST_SUITE_P(
    GrahamScanCases, ConvexHullTest,
    ::testing::Values(ConvexHullParam{{}, {}, false}, ConvexHullParam{{{0, 0}}, {}, false},
                      ConvexHullParam{{{0, 0}, {1, 1}}, {}, false},
                      ConvexHullParam{{{0, 0}, {1, 0}, {0, 1}}, {{0, 0}, {1, 0}, {0, 1}}, true},
                      ConvexHullParam{{{0, 0}, {1, 0}, {1, 1}, {0, 1}}, {{0, 0}, {1, 0}, {1, 1}, {0, 1}}, true},
                      ConvexHullParam{{{0, 0}, {2, 0}, {1, 1}, {1, 0.5}}, {{0, 0}, {2, 0}, {1, 1}}, true}));
