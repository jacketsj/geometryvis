#pragma once

#include <sl.h>
#include <sstream>
#include <string>
#include <vector>

#include "geometry.h"

class pt : public geometry {
private:
	pt operator-(const pt& oth) const { return pt(x - oth.x, y - oth.y); }

public:
	int x, y;
	pt(int x, int y) : x(x), y(y) {}
	int norm2() const { return x * x + y * y; }
	friend int dist2(const pt& a, const pt& b) { return (a - b).norm2(); }
	virtual std::vector<int> comp_list() const { return {x, y}; }
	virtual void draw() const { slPoint(x, y); }
	std::string to_string() const {
		std::stringstream ss;
		ss << "(" << x << ',' << y << ")";
		return ss.str();
	}
};
