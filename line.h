#pragma once

#include <sl.h>
#include <vector>

#include "geometry.h"
#include "point.h"

class line_segment : public geometry {
public:
	pt p1, p2;
	line_segment(pt p1, pt p2) : p1(p1), p2(p2) {}
	static std::string name() { return "line segment"; }
	virtual void draw() const { slLine(p1.x, p1.y, p2.x, p2.y); }
	virtual std::vector<int> comp_list() const {
		std::vector<int> ret = p1.comp_list();
		std::vector<int> p2_cl = p2.comp_list();
		ret.insert(ret.end(), p2_cl.begin(), p2_cl.end());
		return ret;
	}
};
