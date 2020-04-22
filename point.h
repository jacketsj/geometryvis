#pragma once

#include <sl.h>
#include <sstream>
#include <string>
#include <vector>

#include "geometry.h"

template <typename D> class pt : public geometry<D> {
protected:
	virtual pt* clone_impl() const { return new pt(*this); }

public:
	D x, y;
	pt(D x, D y) : x(x), y(y) {}
	pt() : x(0), y(0) {}
	static std::string name() { return "point"; }

	friend D dp(const pt& a, const pt& b) { return a.x * b.x + a.y * b.y; }
	friend D cp(const pt& a, const pt& b) { return a.x * b.y - a.y * b.x; }
	pt operator-(const pt& oth) const { return pt(x - oth.x, y - oth.y); }
	pt operator+(const pt& oth) const { return pt(x + oth.x, y + oth.y); }
	D norm2() const { return x * x + y * y; }
	D norm() const { return sqrt(norm2()); }
	friend D dist2(const pt& a, const pt& b) { return (a - b).norm2(); }
	friend D dist(const pt& a, const pt& b) { return sqrt(dist2(a, b)); }
	friend D turn(const pt& a, const pt& b, const pt& c, D eps) {
		if (dist(a, b) <= eps || dist(b, c) <= eps)
			return 0;
		return cp(b - a, c - a);
	}
	friend int dir(const pt& a, const pt& b, const pt& c, D eps) {
		// first or second line is approximately a point
		D rot = turn(a, b, c, eps);
		if (abs(rot) <= eps)
			return 0;
		else if (rot > 0)
			return 1;
		else
			return -1;
	}

	virtual std::vector<D> comp_list() const { return {x, y}; }
	virtual void draw() const {
		// slPoint(x, y);
		slCircleFill(x, y, 4, 8);
	}
	std::string to_string() const {
		std::stringstream ss;
		ss << "(" << x << ',' << y << ")";
		return ss.str();
	}
};
