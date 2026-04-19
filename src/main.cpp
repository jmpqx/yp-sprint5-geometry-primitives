#include "convex_hull.hpp"
#include "geometry.hpp"
#include "intersections.hpp"
#include "queries.hpp"
#include "shape_utils.hpp"
#include "triangulation.hpp"
#include "visualization.hpp"

#include <algorithm>
#include <functional>
#include <print>
#include <random>
#include <ranges>
#include <variant>

using namespace geometry;

namespace rs = std::ranges;
namespace vs = std::ranges::views;

void PrintAllIntersections(const Shape &shape, std::span<const Shape> others) {
    std::println("\n=== Intersections ===");

    /*
     * Используйте ranges чтобы оставить только фигуры,
     * поддерживающие возможность находить пересечения между собой
     *
     * Затем примените монадический интерфейс для обработки результатов:
     *     - Пересечение найдено в точке A между фигурами B и C
     *     - Фигуры B и C не пересекаются
     */

    if (!std::holds_alternative<Line>(shape) && !std::holds_alternative<Circle>(shape)) {
        throw std::runtime_error("Intersections are only supported for Line and Circle shapes.");
    }

    auto supported_shapes = others | vs::filter([&](const auto &s) {
                                return (std::holds_alternative<Line>(s) || std::holds_alternative<Circle>(s));
                            });

    rs::for_each(supported_shapes, [&shape](const auto &other) {
        auto result_msg =
            intersections::GetIntersectPoint(shape, other)
                .transform([&](const auto &point) {
                    return std::format("Пересечение найдено в точке {} между фигурами {} и {}", point, shape, other);
                })
                .value_or(std::format("Фигуры {} и {} не пересекаются", shape, other));

        std::println("{}", result_msg);
    });
}

void PrintDistancesFromPointToShapes(Point2D p, std::span<const Shape> shapes) {
    std::println("\n=== Distance from Point Test ===");

    /*
     * Используйте ranges чтобы выбрать любые 5 фигур из списка.
     * Затем найдите расстояния от заданной точки до всех выбранных фигур.
     * Выведите результат в формате "Расстояние от точки P до фигуры S равно D"
     */

    std::vector<std::reference_wrapper<const Shape>> selected_shapes;
    selected_shapes.reserve(5);

    rs::sample(shapes, std::back_inserter(selected_shapes), 5, std::mt19937{std::random_device{}()});

    rs::for_each(selected_shapes, [&p](const auto &shape) {
        auto distance = queries::DistanceToPoint(shape, p);
        std::println("Расстояние от точки {} до фигуры {} равно {:.2f}", p, shape.get(), distance);
    });
}

void PerformShapeAnalysis(std::span<const Shape> shapes) {
    std::println("\n=== Shape Analysis ===");

    /*
     * Используйте ranges и созданные классы чтобы:
     *     - Найти все пересечения между фигурами используя метод Bounding Box
     *     - Найти самую высокую фигуру (чья высота наибольшая)expected
     *     - Вывести расстояние между любыми двумя фигурами, которые поддерживают данную функциональность
     */

    rs::for_each(utils::FindAllCollisions(shapes), [](const auto &pair) {
        const auto &[shape1, shape2] = pair;
        std::println("Фигуры {} и {} пересекаются (по Bounding Box)", shape1, shape2);
    });

    if (auto idx = utils::FindHighestShape(shapes)) {
        std::println("Самая высокая фигура: {}", shapes.at(idx.value()));
    }

    auto supported_shapes = shapes | vs::filter([](const auto &s) {
                                return std::holds_alternative<Line>(s) || std::holds_alternative<Circle>(s);
                            });

    std::vector<std::reference_wrapper<const Shape>> selected_shapes;
    rs::sample(supported_shapes, std::back_inserter(selected_shapes), 2, std::mt19937{std::random_device{}()});

    if (auto distance = queries::DistanceBetweenShapes(selected_shapes[0], selected_shapes[1])) {
        std::println("Расстояние между фигурами {} и {} равно {:.2f}", selected_shapes[0].get(),
                     selected_shapes[1].get(), *distance);
    } else {
        std::println("Расстояние между фигурами {} и {} не может быть вычислено", selected_shapes[0].get(),
                     selected_shapes[1].get());
    }
}

void PerformExtraShapeAnalysis(std::span<const Shape> shapes) {
    std::println("\n=== Shape Extra Analysis ===");

    /*
     * Используйте ranges и созданные классы чтобы:
     *     - Вывести 3 любые фигуры, которые находятся выше 50.0
     *     - Вывести фигуры с наименьшей и с наибольшей высотами
     */

    auto heights_and_shapes =
        vs::zip(shapes, shapes | vs::transform(queries::GetHeight)) | vs::transform([](const auto &pair) {
            const auto &[shape, height] = pair;
            return std::pair{shape, height};
        });

    const auto &[min, max] = rs::minmax_element(heights_and_shapes, {}, [](const auto &pair) { return pair.second; });
    std::println("Фигура с наименьшей высотой: {}", *min);
    std::println("Фигура с наибольшей высотой: {}", *max);

    auto above_threshold =
        heights_and_shapes | vs::filter([](const auto &pair) { return pair.second > 50.0; }) | vs::take(3);
    rs::for_each(above_threshold,
                 [](const auto &pair) { std::println("Фигура {} находится выше 50.0: {}", pair.first, pair.second); });
}

int main() {
    std::vector<Shape> shapes = utils::ParseShapes("circle 0 0 1.5; line 1 2 3 4; polygon 0 0 2 5; triangle 0 0 1 0 "
                                                   "0.5 1; polygon 0 0 1 2; badshape; circle 0 0 -1");
    std::println("Parsed {} shapes", shapes.size());

    // Выведите индекс каждой фигуры и её высоту
    std::println("Info: {}", shapes);
    //
    // Вызываем разработанные функции
    //
    PrintAllIntersections(shapes[0], shapes);

    PrintDistancesFromPointToShapes(Point2D{10.0, 10.0}, shapes);

    PerformShapeAnalysis(shapes);

    PerformExtraShapeAnalysis(shapes);

    //
    // Рисуем все фигуры
    //
    // Важно: после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 2ой график
    //
    visualization::Draw(shapes);

    //
    // Формируем список из вершин всех фигур
    //
    std::vector<Point2D> points;

    rs::for_each(shapes, [&points](const auto &shape) {
        std::visit(
            [&points](const auto &s) {
                const auto verts = s.Vertices();
                points.insert(points.end(), verts.begin(), verts.end());
            },
            shape);
    });

    //
    // Находим список точек, для построения выпуклой оболочки - convex hull - алгоритмом Грэхема
    // Создаём из них объект класса `Polygon` и добавляем его в список shapes
    // Рисуем все фигуры
    //

    auto convex_hull = convex_hull::GrahamScan(points);
    if (convex_hull) {
        Polygon hull_polygon{convex_hull.value()};
        shapes.push_back(hull_polygon);
        visualization::Draw(shapes);
    }

    //
    // после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 3ий график
    //

    {
        std::vector<Point2D> points = {{0, 0}, {10, 0}, {5, 8}, {15, 5}, {2, 12}};

        //
        // Используйте список точек points или свой, чтобы
        // выполнить алгоритм триангуляции Делоне алгоритмом Боуэра-Ватсона
        //
        // После успешного завершения алгоритма - выведите результат для проверки
        // используя geometry::visualization::Draw
        //

        auto triangles = triangulation::DelaunayTriangulation(points);
        if (triangles) {
            visualization::Draw(triangles.value());
        }
    }

    return 0;
}
