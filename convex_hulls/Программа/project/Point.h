#pragma once

struct Point
{
	double x, y;

	constexpr explicit Point(double x = 0, double y = 0) : x(x), y(y) {}

	bool operator==(const Point r) const {
		return this->x == r.x && this->y == r.y;
	}

	bool operator!=(const Point r) const {
		return !operator==(r);
	}

};