#include "line.h"

#include "geometry.h"
#include "point.h"
#include "properties.h"
#include "render_adapter.h"

template <> void line_segment<double>::draw() const {
	render_adapter::draw_line(p1, p2, this->prop);
}

template <>
std::vector<pt<double>>
line_segment<double>::isect(const line_segment<double>& oth) const {
	// based on UBC ICPC code archive seg_x_seg algorithm:
	// https://www.dropbox.com/s/0g83z1wh7bmwdu5/codearchive.cpp
	const double eps = 1e-7;
	const static bool isect_endpoints = false;

	std::vector<pt<double>> ret;

	// define a canonical order on points
	auto cmp_lex = [&](const pt<double>& a, const pt<double>& b) {
		// ordered first by y coordinate, then by x coordinate
		if ((a.x - b.x) <= 2 * eps)
			return a.y < b.y - eps;
		return a.x < b.x - eps;
	};

	bool does_intersect = false;
	// easy notation
	pt<double> l1p1 = p1, l1p2 = p2, l2p1 = oth.p1, l2p2 = oth.p2;

	// if either line is just a point, and endpoints cannot intersect
	if (!isect_endpoints &&
			(dist2(l1p1, l1p2) <= eps || dist2(l2p1, l2p2) <= eps)) {
		does_intersect = false;
		return ret;
	}

	// the four direction checks
	int dir1 = dir(l1p1, l1p2, l2p1, eps), dir2 = dir(l1p1, l1p2, l2p2, eps),
			dir3 = dir(l2p1, l2p2, l1p1, eps), dir4 = dir(l2p1, l2p2, l1p2, eps);
	if (dir1 == 0 && dir2 == 0 && dir3 == 0 && dir4 == 0) { // collinear
		if (cmp_lex(l1p2, l1p1))
			std::swap(l1p1, l1p2);
		if (cmp_lex(l2p2, l2p1))
			std::swap(l2p1, l2p2);
		if (isect_endpoints)
			does_intersect = !cmp_lex(l2p2, l1p1) && !cmp_lex(l1p2, l2p1);
		else
			does_intersect = cmp_lex(l1p1, l2p2) && cmp_lex(l2p1, l1p2);
		if (does_intersect) {
			// find the bounds of the intersection
			pt<double>& i1 = l1p1; // lexical rightmost of the left endpoints
			pt<double>& i2 = l1p2; // lexical leftmost of the right endpoints
			if (cmp_lex(l1p1, l2p1))
				i1 = l2p1;
			if (cmp_lex(l2p2, l1p2))
				i2 = l2p2;
			ret.push_back(i1);
			ret.push_back(i2);
		}
	} else {
		if (isect_endpoints)
			does_intersect = dir1 * dir2 <= 0 && dir3 * dir4 <= 0;
		else
			does_intersect = dir1 * dir2 < 0 && dir3 * dir4 < 0;
		if (does_intersect) {
			// find the intersection of the full lines
			// y = (s/n)*x + b
			// so n*y-s*x=n*b
			// s = y1-y2, n = x1-x2
			// for the other line, we get n'*y-s'*x=n'*b'
			// elimination gives:
			// (n-s*n'/s')*y = n*b - n'*b'*s/s'
			// (s'*n/n'-s)*x = n*b - n'*b'*n/n'
			double n1 = l1p1.x - l1p2.x, s1 = l1p1.y - l1p2.y;
			double nb1 = n1 * l1p1.y - s1 * l1p1.x;
			double n2 = l2p1.x - l2p2.x, s2 = l2p1.y - l2p2.y;
			double nb2 = n2 * l2p1.y - s2 * l2p1.x;
			double x, y;
			if (abs(s1) < eps)
				y = l1p1.y;
			else if (abs(s2) < eps)
				y = l2p1.y;
			else {
				double y_denom = n1 - s1 * n2 / s2;
				double y_num = nb1 - nb2 * s1 / s2;
				y = y_num / y_denom;
			}
			if (abs(n1) < eps)
				x = l1p1.x;
			else if (abs(n2) < eps)
				x = l2p1.x;
			else {
				double x_denom = s2 * n1 / n2 - s1;
				double x_num = nb1 - nb2 * n1 / n2;
				x = x_num / x_denom;
			}
			ret.emplace_back(x, y);
		}
	}

	return ret;
}
