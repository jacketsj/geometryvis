#pragma once

#include <memory>
#include <tuple>
#include <vector>

#include "circle.h"
#include "geometry.h"
#include "line.h"
#include "tool_algo_analysis.h"

template <typename D>
class analysis_convex_hull_circles : public tool_algo_analysis<circle<D>, D> {
private:
	virtual std::vector<std::unique_ptr<geometry<D>>>
	analyse(std::vector<std::reference_wrapper<const circle<D>>>&& circles_ref) {
		const D eps = D(1e-7);
		std::vector<circle<D>> circles;
		circles.reserve(circles_ref.size());
		for (const circle<D>& p : circles_ref)
			circles.push_back(p);
		std::vector<std::unique_ptr<geometry<D>>> ret;
		if (circles.size() <= 1)
			return ret;
		auto remove = [&](size_t i) {
			std::swap(circles[i], circles.back());
			circles.pop_back();
		};
		// find leftmost point (break ties with bottommost leftmost)
		size_t prev = 0;
		for (size_t i = 1; i < circles.size(); ++i) {
			circle<D>& c = circles[i];
			circle<D>& c_prev = circles[prev];
			if (std::make_tuple(c.p.x - c.rad(), c.p.y) <
					std::make_tuple(c_prev.p.x - c_prev.rad(), c_prev.p.y))
				prev = i;
		}
		// basic O(nh) Jarvis march with circles tangents
		// bugs: wrong tangent thing, also skipping circles due to intersections
		// that are still outside
		std::vector<circle<D>> hull;
		do {
			hull.push_back(circles[prev]);
			// if (hull.size() > 1)
			remove(prev);
			circle<D>& last = hull.back();
			size_t next = 0;
			// is a better than b
			auto better_circle = [&](circle<D>& a, circle<D>& b) {
				// if (prev == i && hull.size() <= 1)
				// 	continue;
				if (a.inside(b, eps))
					return false;
				else if (b.inside(a, eps))
					return true;
				int d = dir(pt<D>::origin(), a.tangent_line_dif(last),
										b.tangent_line_dif(last), eps);
				if (d == 0) {
					if (a.tangent_line_dif(last).norm2() >
							b.tangent_line_dif(last).norm2()) {
						return true;
					}
				} else if (d < 0) {
					return true;
				}
				return false;
			};
			for (size_t i = 1; i < circles.size(); ++i) {
				if (better_circle(circles[i], circles[next]))
					next = i;
			}
			if (better_circle(hull[0], circles[next]))
				break;
			prev = next;
		} while (circles.size() > 0 && dist2(circles[prev].p, hull[0].p) > eps);
		for (size_t i = 1; i < hull.size(); ++i)
			ret.push_back(
					std::make_unique<line_segment<D>>(hull[i].tangent_line(hull[i - 1])));
		ret.push_back(std::make_unique<line_segment<D>>(
				hull[0].tangent_line(hull[hull.size() - 1])));
		console::get().print("number of segments in hull: " +
												 std::to_string(hull.size()));
		return ret;
	}

protected:
	virtual std::string name() { return "convex hull algorithm for circles"; }

public:
	analysis_convex_hull_circles() { this->print_tool_name(name()); }
};
