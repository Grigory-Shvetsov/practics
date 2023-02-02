#include <iostream>
#include <algorithm>
#include <iterator>

#include "KirkpatrickSeidelAlgorithm.h"

template<typename T>
T KSAlgorithm::medianNonConst(std::vector<T>& vec) {
	T med{};

	if (vec.empty())
		return med;

	if (vec.size() == 1)
		return vec[0];

	//return KSAlgorithm::median_of_medians(vec);

	size_t size = vec.size();
	std::nth_element(vec.begin(), vec.begin() + (size / 2), vec.end());
	if (size % 2 == 1)
		med = vec[size / 2];
	else
		med = (vec[size / 2 - 1] + vec[size / 2]) / 2;
	return med;
}

template<typename T>
T KSAlgorithm::median(const std::vector<T>& vec) {
	std::vector<T> ls(vec.begin(), vec.end());
	return medianNonConst(ls);
}

double KSAlgorithm::medianPointX(const std::vector<Point>& vec) {
	std::vector<double> ls;
	ls.reserve(vec.size());
	for (auto p : vec)
		ls.push_back(p.x);

	return medianNonConst(ls);
}

std::vector<Point> KSAlgorithm::convexHull(const std::vector<Point>& src) {
	std::vector<Point> upper = upperHull<true>(src);

	// чтобы не писать один алгоритм дважды, мы просто
	// отражаем картинку и снова рассматриваем верхнюю границу
	//std::vector<Point> lower = flipped(upperHull<true>(flipped(src)));
	std::vector<Point> lower = upperHull<false>(src);
	std::reverse(lower.begin(), lower.end());

	if (upper.back() == lower.front())
		upper.pop_back();

	if (upper.front() == lower.back())
		lower.pop_back();

	return joinVectors(std::move(upper), lower);
}

template<bool UP>
std::vector<Point> KSAlgorithm::upperHull(const std::vector<Point>& points) {
	Point min = points[0];
	Point max = points[1];

	for (auto point : points) {
		//if ((point.x < min.x) || (point.x == min.x && point.y > min.y))
		if ((point.x < min.x) || (dequal(point.x, min.x) && ((UP) ? (point.y > min.y) : (point.y < min.y))))
			min = point;

		//if ((point.x > max.x) || (point.x == max.x && point.y > max.y))
		if ((point.x > max.x) || (dequal(point.x, max.x) && ((UP) ? (point.y > max.y) : (point.y < max.y))))
			max = point;
	}

	if (min == max)
		return std::vector<Point>{min};

	std::vector<Point> newPoints;
	newPoints.reserve(points.size());
	newPoints.push_back(min);
	if (UP) {
		double minY = std::min(min.y, max.y);
		std::copy_if(points.begin(), points.end(), std::back_inserter(newPoints), [min, max, minY](Point p) {
			return (min.x < p.x&& p.x < max.x) && (minY <= p.y);
			});
	}
	else {
		double maxY = std::max(min.y, max.y);
		std::copy_if(points.begin(), points.end(), std::back_inserter(newPoints), [min, max, maxY](Point p) {
			return (min.x < p.x&& p.x < max.x) && (p.y <= maxY);
			});
	}
	newPoints.push_back(max);

	return connect<UP>(min, max, newPoints);
}

template<bool UP>
std::vector<Point> KSAlgorithm::connect(Point min, Point max, std::vector<Point>& points) {
	if (min == max) {
		return std::vector<Point>{min};
	}
	double med = medianPointX(points);

	// находим "мост"
	std::pair<Point, Point> bridgePoints = bridge<UP>(points, med);
	auto left = bridgePoints.first;
	auto right = bridgePoints.second;

	std::vector<Point> sLeft;
	std::vector<Point> sRight;
	sLeft.reserve(points.size() / 2);
	sRight.reserve(points.size() / 2);

	sRight.push_back(right);

	// заполняем sLeft и sRight
	for (auto point : points) {
		if (point.x < left.x)
			sLeft.push_back(point);

		if (point.x > right.x)
			sRight.push_back(point);
	}
	sLeft.push_back(left);

	sLeft.shrink_to_fit();
	sRight.shrink_to_fit();

	return joinVectors((min == left) ? std::vector<Point>{min} : connect<UP>(min, left, sLeft),
		(max == right) ? std::vector<Point>{max} : connect<UP>(right, max, sRight));
}

// поиск "моста"
template<bool UP>
std::pair<Point, Point> KSAlgorithm::bridge(const std::vector<Point>& points, double xMedian) {
	if (points.empty() || points.size() == 1)
		throw std::runtime_error("there must be at least two points");

	if (points.size() == 2) {
		if (points[0].x < points[1].x) return std::make_pair(points[0], points[1]);
		else return std::make_pair(points[1], points[0]);
	}

	std::vector<Point> candidates;

	// шаги 2 и 3
	// TODO занимает большое кол-во памяти
	//std::vector<std::pair<Point, Point>> pairs;
	//pairs.reserve(points.size() / 2);
	std::vector<double> slopes;
	slopes.reserve(points.size() / 2);

	if (points.size() % 2 == 1)
		candidates.push_back(points.back());

	for (size_t i = 0; i < points.size() / 2; ++i) {
		Point pi = points[i * 2];
		Point pj = points[i * 2 + 1];

		if (dequal(pi.x, pj.x)) {
			//candidates.push_back((pi.y > pj.y) ? pi : pj);
			candidates.push_back((UP ? (pi.y > pj.y) : (pi.y < pj.y)) ? pi : pj);
		}
		else {

			slopes.push_back((pi.y - pj.y) / (pi.x - pj.x));
		}
	}

	// 4 шаг
	//double medianSlope = median(slopes);
	double medianSlope = medianNonConst(slopes);
	slopes.~vector();

	// 5 шаг

	// 6 шаг
	// TODO теперь не совсем max
	double maxSlope = points[0].y - medianSlope * points[0].x;
	for (size_t i = 1; i < points.size(); ++i)
		//maxSlope = std::max(maxSlope, points[i].y - medianSlope * points[i].x);
		maxSlope = UP ? (std::max(maxSlope, points[i].y - medianSlope * points[i].x)) : (std::min(maxSlope, points[i].y - medianSlope * points[i].x));

	std::vector<Point> maxVect;
	std::copy_if(points.begin(), points.end(), std::back_inserter(maxVect), [medianSlope, maxSlope](Point p) {
		return dequal(p.y - medianSlope * p.x, maxSlope);
		});

	static std::function<bool(Point, Point)> comparePointX = [](Point l, Point r) {
		return l.x < r.x;
	};

	Point left = *std::min_element(maxVect.begin(), maxVect.end(), comparePointX);
	Point right = *std::max_element(maxVect.begin(), maxVect.end(), comparePointX);

	// 7 шаг
	//if (left.x <= xMedian && right.x > xMedian)
	if ((dequal(left.x, xMedian) || left.x < xMedian) && right.x > xMedian)
		return std::make_pair(left, right);
	// 8 шаг
	//else if (right.x <= xMedian) {
	else if ((dequal(right.x, xMedian) || right.x < xMedian)) {
		candidates.reserve(points.size() * 3 / 4);
		for (size_t i = 0; i < points.size() / 2; ++i) {
			Point pi = points[i * 2];
			Point pj = points[i * 2 + 1];

			if (!dequal(pi.x, pj.x)) {
				double slope = (pi.y - pj.y) / (pi.x - pj.x);

				if (UP)
					if (dequal(slope, medianSlope) || slope > medianSlope) {
						candidates.push_back((pi.x > pj.x) ? pi : pj);
					}
					else {
						candidates.push_back(pi);
						candidates.push_back(pj);
					}
				else
					if (dequal(slope, medianSlope) || slope < medianSlope) {
						//candidates.push_back((pi.x < pj.x) ? pi : pj);
						candidates.push_back((pi.x > pj.x) ? pi : pj);
					}
					else {
						candidates.push_back(pi);
						candidates.push_back(pj);
					}

			}
		}
	}
	// 9 шаг
	else {

		for (size_t i = 0; i < points.size() / 2; ++i) {
			Point pi = points[i * 2];
			Point pj = points[i * 2 + 1];

			if (!dequal(pi.x, pj.x)) {
				double slope = (pi.y - pj.y) / (pi.x - pj.x);

				if (UP)
					if (dequal(slope, medianSlope) || slope < medianSlope) {
						candidates.push_back((pi.x < pj.x) ? pi : pj);
					}
					else {
						candidates.push_back(pi);
						candidates.push_back(pj);
					}
				else
					if (dequal(slope, medianSlope) || slope > medianSlope) {
						//candidates.push_back((pi.x > pj.x) ? pi : pj);
						candidates.push_back((pi.x < pj.x) ? pi : pj);
					}
					else {
						candidates.push_back(pi);
						candidates.push_back(pj);
					}

			}
		}
	}

	return bridge<UP>(candidates, xMedian);
}

template<typename T>
T KSAlgorithm::median_of_medians(std::vector<T>& vec, size_t count_in_group) {
	T pivot{}; // initialized with zero
	//std::vector<T> m;
	size_t r = vec.size();

	if (r == 0)
		return pivot;

	size_t counter = 0;

	for (size_t i = 0; i < r; i += count_in_group) {
		std::sort(vec.begin() + i, vec.begin() + std::min(r, i + count_in_group));
		size_t mid = (i + std::min(r, i + count_in_group)) / 2;
		//m.push_back(vec[mid]);
		vec[counter++] = vec[mid];
	}
	vec.resize(counter);
	//size_t sz = m.size();
	size_t sz = counter;
	if (sz <= count_in_group) {
		//std::sort(m.begin(), m.end());
		std::sort(vec.begin(), vec.end());
		//pivot = m[(sz - 1) / 2];
		pivot = vec[(sz - 1) / 2];
	}
	else {
		//pivot = median_of_medians(m);
		pivot = median_of_medians(vec);
	}
	return pivot;
}
