#pragma once

#include <cmath>
#include <sstream>
#include <string>
#include <vector>

#include "basic_point.h"
#include "basic_rect.h"
#include "geometry.h"
#include "properties.h"

template <typename D> class pt : public geometry<D> {
private:
	pt<D> preserve_prop(pt<D> p) {
		p.prop = this->prop;
		return p;
	}

protected:
	virtual pt* clone_impl() const { return new pt(*this); }

public:
	D x, y;

	pt(D x, D y) : x(x), y(y) {}
	pt() : x(0), y(0) {}
	// pt(const basic_pt<D>& p) : x(p.x), y(p.y) {}

	static std::string name() { return "point"; }
	static pt<D> from_basic_pt(const basic_pt<D>& bp) {
		return pt<D>(bp.x, bp.y);
	}

	friend D dp(const pt& a, const pt& b) { return a.x * b.x + a.y * b.y; }
	friend D cp(const pt& a, const pt& b) { return a.x * b.y - a.y * b.x; }
	pt operator+(const pt& oth) const { return pt(x + oth.x, y + oth.y); }
	pt operator-(const pt& oth) const { return pt(x - oth.x, y - oth.y); }
	pt operator-() const { return pt(-x, -y); }
	pt operator*(const D& scale) const { return pt(x * scale, y * scale); }
	pt operator/(const D& scale) const { return pt(x / scale, y / scale); }
	pt operator*(const pt<D>& oth) const {
		return pt(x * oth.x - y * oth.y, x * oth.y + y * oth.x);
	}
	void operator+=(const pt& oth) { (*this) = preserve_prop((*this) + oth); }
	void operator-=(const pt& oth) { (*this) = preserve_prop((*this) - oth); }
	void operator*=(const D& scale) { (*this) = preserve_prop((*this) * scale); }
	void operator/=(const D& scale) { (*this) = preserve_prop((*this) / scale); }
	void operator*=(const pt<D>& oth) { (*this) = preserve_prop((*this) * oth); }
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

	pt ortho_cw() const { return pt(y, -x); }
	pt ortho_ccw() const { return pt(-y, x); }
	pt normalize() const { return (*this) / norm(); }

	static pt origin() { return pt(0, 0); }

	virtual std::vector<D> comp_list() const { return {x, y}; }
	virtual void draw() const;

	std::string to_string() const {
		std::stringstream ss;
		ss << "(" << x << ',' << y << ")";
		return ss.str();
	}

	virtual basic_rect<D> bounding_box() const {
		return basic_rect<D>(x, y, 16, 16);
	}

	void operator+=(const basic_pt<D>& oth) {
		(*this) = preserve_prop((*this) + from_basic_pt(oth));
	}
	virtual void translate(const basic_pt<D>& delta) { (*this) += delta; }

	basic_pt<D> to_basic() const { return basic_pt<D>(x, y); }
};
