#pragma once
#include "geometry.hpp"
#include <cmath>
#include <optional>

namespace geometry::intersections {

/*
 * Класс для поиска пересечений между двумя фигурами
 *
 * Требуется организовать возможность нахождения пересечений только для следующих комбинаций фигур:
 *    - Line   & Line
 *    - Line   & Circle
 *    - Circle & Circle
 *
 * Для всех остальных требуется выбросить исключение std::logic_error
 */
class IntersectionVisitor {
public:
    static constexpr double epsilon = 1e-9;

    [[nodiscard]] std::optional<Point2D> operator()(const Line &line1, const Line &line2) const;
    [[nodiscard]] std::optional<Point2D> operator()(const Line &line, const Circle &circle) const;
    [[nodiscard]] std::optional<Point2D> operator()(const Circle &circle, const Line &line) const;
    [[nodiscard]] std::optional<Point2D> operator()(const Circle &circle1, const Circle &circle2) const;

    [[nodiscard]] std::optional<Point2D> operator()(const auto &, const auto &) const {
        throw std::logic_error("Intersection not supported for these shapes");
    }
};

inline std::optional<Point2D> IntersectionVisitor::operator()(const Line &line1, const Line &line2) const {
    if (!line1.BoundBox().Overlaps(line2.BoundBox())) {
        return std::nullopt;
    }

    Point2D d1 = line1.end - line1.start;
    Point2D d2 = line2.end - line2.start;
    double cross = d1.Cross(d2);

    if (std::abs(cross) < epsilon) {
        return std::nullopt;
    }

    Point2D delta = line2.start - line1.start;
    double t = delta.Cross(d2) / cross;
    double u = delta.Cross(d1) / cross;

    if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
        return line1.start + d1 * t;
    }

    return std::nullopt;
}

inline std::optional<Point2D> IntersectionVisitor::operator()(const Line &line, const Circle &circle) const {
    if (!line.BoundBox().Overlaps(circle.BoundBox())) {
        return std::nullopt;
    }

    Point2D d = line.end - line.start;
    Point2D f = line.start - circle.center_p;

    double dist = std::abs(f.Cross(d)) / d.Length();
    if (dist > circle.radius) {
        return std::nullopt;
    }

    double a = d.Dot(d);
    double b = 2.0 * f.Dot(d);
    double c = f.Dot(f) - circle.radius * circle.radius;
    double discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0) {
        return std::nullopt;
    }

    double sqrt_disc = std::sqrt(discriminant);
    double t1 = (-b - sqrt_disc) / (2.0 * a);
    double t2 = (-b + sqrt_disc) / (2.0 * a);

    if (t1 >= 0.0 && t1 <= 1.0) {
        return line.start + d * t1;
    }
    if (t2 >= 0.0 && t2 <= 1.0) {
        return line.start + d * t2;
    }

    return std::nullopt;
}

inline std::optional<Point2D> IntersectionVisitor::operator()(const Circle &circle, const Line &line) const {
    return (*this)(line, circle);
}

inline std::optional<Point2D> IntersectionVisitor::operator()(const Circle &circle1, const Circle &circle2) const {
    if (!circle1.BoundBox().Overlaps(circle2.BoundBox())) {
        return std::nullopt;
    }

    Point2D delta = circle2.center_p - circle1.center_p;
    double dist = delta.Length();

    if (dist > circle1.radius + circle2.radius || dist < std::abs(circle1.radius - circle2.radius) || dist < epsilon) {
        return std::nullopt;
    }

    double a = (circle1.radius * circle1.radius - circle2.radius * circle2.radius + dist * dist) / (2.0 * dist);

    Point2D dir = delta / dist;
    Point2D mid = circle1.center_p + dir * a;

    double h = std::sqrt(circle1.radius * circle1.radius - a * a);
    Point2D perp = {-dir.y, dir.x};

    return mid + perp * h;
}

inline std::optional<Point2D> GetIntersectPoint(const Shape &shape1, const Shape &shape2) {
    return std::visit(IntersectionVisitor{}, shape1, shape2);
}

}  // namespace geometry::intersections