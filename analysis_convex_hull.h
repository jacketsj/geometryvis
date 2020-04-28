#pragma once

#include <memory>
#include <tuple>
#include <vector>

#include "circle.h"
#include "geometry.h"
#include "line.h"
#include "tool_algo_analysis.h"

template <typename D>
class analysis_convex_hull : public tool_algo_analysis<pt<D>, D> {
private:
	virtual std::vector<std::unique_ptr<geometry<D>>>
	analyse(std::vector<std::reference_wrapper<const pt<D>>>&& points_ref) {
		const D eps = D(1e-7);
		std::vector<pt<D>> points;
		points.reserve(points_ref.size());
		for (const pt<D>& p : points_ref)
			points.push_back(p);
		std::vector<std::unique_ptr<geometry<D>>> ret;
		if (points.size() <= 1)
			return ret;
		// basic O(nh) Jarvis march
		// find leftmost point (break ties with bottommost leftmost)
		auto remove = [&](size_t i) {
			std::swap(points[i], points.back());
			points.pop_back();
		};
		pt<D> prev = points[0];
		for (pt<D>& p : points) {
			if (std::tie(p.x, p.y) < std::tie(prev.x, prev.y))
				prev = p;
		}
		std::vector<pt<D>> hull;
		do {
			hull.push_back(prev);
			size_t next = 0;
			for (size_t i = 1; i < points.size(); ++i) {
				int d = dir(prev, points[i], points[next], eps);
				if (d == 0) {
					if (dist2(points[i], prev) > dist2(points[next], prev)) {
						next = i;
					}
				} else if (d < 0) {
					next = i;
				}
			}
			prev = points[next];
			remove(next);
		} while (points.size() > 0 && dist2(prev, hull[0]) > eps);
		for (size_t i = 1; i < hull.size(); ++i)
			ret.push_back(std::make_unique<line_segment<D>>(hull[i - 1], hull[i]));
		if (hull.size() > 2)
			ret.push_back(
					std::make_unique<line_segment<D>>(hull[hull.size() - 1], hull[0]));
		return ret;
	}

protected:
	virtual std::string name() { return "convex hull algorithm"; }

public:
	analysis_convex_hull() { this->print_tool_name(name()); }
};
