#pragma once

#include <cmath>
#include <sl.h>
#include <string>
#include <vector>

#include "geometry.h"
#include "point.h"

class circle : public geometry {
private:
	bool isect(const pt& p0) const { return dist2(p, p0) <= r2; }
	bool isect(int x0, int y0) const { return isect(pt(x0, y0)); }

public:
	pt p;
	int r2;
	circle(int x, int y, int r2) : p(x, y), r2(r2) {}
	circle(const pt& p, int r2) : p(p), r2(r2) {}
	circle(const pt& p, const pt& pr) : p(p), r2(dist2(pr, p)) {}
	static std::string name() { return "circle"; }
	virtual void draw() const { slCircleFill(p.x, p.y, sqrt(double(r2)), r2); }
	virtual std::vector<int> comp_list() const {
		std::vector<int> ret = p.comp_list();
		ret.push_back(r2);
		return ret;
	}
};
