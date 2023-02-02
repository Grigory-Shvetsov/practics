#ifndef SIMPLECONVEXHULL_H
#define SIMPLECONVEXHULL_H

#include "Point.h"

#include "functional"
#include "algorithm"
#include <vector>

#include <iostream>

namespace SimpleAlgorithm {

	namespace {
		// сравнивает два числа double с учетом погрешности
		// возвращает true, если числа равны (с определенной погрешностью)
		bool dequal(double d1, double d2) {
			/*const double eps = std::min(fabs(d1), fabs(d2)) * 1e-10;
			return fabs(d1 - d2) < eps;*/
			static double eps = 1e-12;
			return fabs(d1 - d2) <= eps;
		}

		bool isSide(const std::vector<Point>& points, Point a1, Point a2)
		{

			int sgn = 0;
			bool flag = true;

			for (size_t i = 0; i < points.size(); ++i)
			{

				Point b = points[i];
				if ((b == a1) || (b == a2))
					continue;


				double coefLeft = (a2.x - a1.x) * (b.y - a2.y);
				double coefRight = (a2.y - a1.y) * (b.x - a2.x);

				if (!dequal(coefLeft, coefRight))
				{
					if (sgn == 0) 
					{
						sgn = (coefLeft > coefRight) ? 1 : -1;
						continue;
					}

					int newSgn = (coefLeft > coefRight) ? 1 : -1;
					if (newSgn != sgn)
					{
						flag = false;
						break;
					}
				}
			}

			return flag;
		}
	}

	std::vector<Point> сonvexHull(const std::vector<Point>& points)
	{
		// частные случаи
		if (points.size() <= 3)
			return points;

		std::vector<Point> res{};

		for (size_t i = 0; i < points.size() - 1; ++i) 
		{
			for (size_t j = i + 1; j < points.size(); ++j) 
			{
				Point a1 = points[i];
				Point a2 = points[j];

				if (a1 != a2)
					if (isSide(points, a1, a2)) 
					{

						if (std::find(res.begin(), res.end(), a1) == res.end())
							res.push_back(a1);

						if (std::find(res.begin(), res.end(), a2) == res.end())
							res.push_back(a2);
					}
			}
		}

		Point a = res[0];
		// TODO сортируем точки по полярному углу
		std::sort(res.begin(), res.end(), [a](Point b1, Point b2) 
			{
			Point c1 = Point{ b1.x - a.x, b1.y - a.y };
			Point c2 = Point{ b2.x - a.x, b2.y - a.y };
			return	atan2(c1.x, c1.y) < atan2(c2.x, c2.y);
			});

		return res;

	}
}

#endif // SIMPLECONVEXHULL
