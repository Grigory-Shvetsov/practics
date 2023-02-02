#ifndef KURS2021_KIRKPATRICKSEIDELALGORITHM_H
#define KURS2021_KIRKPATRICKSEIDELALGORITHM_H

#include <functional>
#include <vector>

#include "Point.h"

class KSAlgorithm {
public:
    static std::vector<Point> convexHull(const std::vector<Point>& src);

private:
    KSAlgorithm() = default;

    template<bool UP>
    static std::pair<Point, Point> bridge(const std::vector<Point>& points, double xMedian);

    template<bool UP>
    static std::vector<Point> connect(Point min, Point max, std::vector<Point>& points);

    template<bool UP>
    static std::vector<Point> upperHull(const std::vector<Point>& points);

    template<typename T>
    static std::vector<T> joinVectors(std::vector<T>&& l, const std::vector<T>& r) {
        l.reserve(l.size() + r.size());

        for (auto& point : r)
            l.push_back(point);

        return l;
    };

    // count_in_group лучше брать нечетным
    template<typename T>
    static T median_of_medians(std::vector<T>& vec, size_t count_in_group = 21);

    template<typename T>
    static T medianNonConst(std::vector<T>& vec);

    template<typename T>
    static T median(const std::vector<T>& vec);

    static double medianPointX(const std::vector<Point>& vec);

    // сравнивает два числа double с учетом погрешности
    // возвращает true, если числа равны (с определенной погрешностью)
    static bool dequal(double d1, double d2) {
        /*const double eps = std::min(fabs(d1), fabs(d2)) * 1e-10;
        return fabs(d1 - d2) < eps;*/
        static double eps = 1e-12;
        return fabs(d1 - d2) < eps;
    };

};

#endif //KURS2021_KIRKPATRICKSEIDELALGORITHM_H
