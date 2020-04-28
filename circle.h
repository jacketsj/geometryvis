#pragma once

#include <cmath>
#include <sl.h>
#include <string>
#include <vector>

#include "geometry.h"
#include "line.h"
#include "point.h"

template <typename D> class circle : public geometry<D> {
private:
	bool isect(const pt<D>& p0) const { return dist2(p, p0) <= r2; }
	bool isect(int x0, int y0) const { return isect(pt<D>(x0, y0)); }

protected:
	virtual circle* clone_impl() const { return new circle(*this); }

public:
	pt<D> p;
	D r2;
	circle(D x, D y, D r2) : p(x, y), r2(r2) {}
	circle(const pt<D>& p, D r2) : p(p), r2(r2) {}
	circle(const pt<D>& p, const pt<D>& pr) : p(p), r2(dist2(pr, p)) {}
	static std::string name() { return "circle"; }
	virtual void draw() const { slCircleOutline(p.x, p.y, sqrt(double(r2)), r2); }
	virtual std::vector<D> comp_list() const {
		std::vector<D> ret = p.comp_list();
		ret.push_back(r2);
		return ret;
	}
	D rad() const { return sqrt(r2); }
	line_segment<D> tangent_line(const circle& origin, bool cw = false) const {
		pt<D> dif_centres_norm = (p - origin.p).normalize();
		if (cw)
			dif_centres_norm = dif_centres_norm.ortho_cw();
		else
			dif_centres_norm = dif_centres_norm.ortho_ccw();
		pt<D> start = origin.p, end = p;
		end += (dif_centres_norm * rad());
		start += (dif_centres_norm * origin.rad());
		return line_segment<D>(start, end);
	}

	pt<D> tangent_line_dif(const circle& origin, bool cw = false) const {
		return tangent_line(origin, cw).dif();
	}

	bool inside(const circle& oth, D eps) const {
		return dist(p, oth.p) < oth.rad() - rad() - eps;
	}
};
