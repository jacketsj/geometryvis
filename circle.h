#pragma once

#include <sl.h>
#include <vector>

#include "geometry.h"
#include "point.h"

class circle : public geometry {
private:
	bool isect(const pt& p0) const { return dist2(p, p0) <= r * r; }
	bool isect(int x0, int y0) const { return isect(pt(x0, y0)); }

public:
	pt p;
	int r;
	circle(int x, int y, int r) : p(x, y), r(r) {}
	virtual void draw() const { slCircleFill(p.x, p.y, r, r); }
	virtual std::vector<int> comp_list() const {
		std::vector<int> ret = p.comp_list();
		ret.push_back(r);
		return ret;
	}
};
