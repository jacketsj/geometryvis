#pragma once

#include <memory>
#include <vector>

#include "circle.h"
#include "geometry.h"
#include "line.h"
#include "tool_algo_analysis.h"

template <typename D>
class analysis_line_intersection
		: public tool_algo_analysis<line_segment<D>, D> {
private:
	const D isect_rad = 32;

	virtual std::vector<std::unique_ptr<geometry<D>>>
	analyse(std::vector<std::reference_wrapper<const line_segment<D>>>&&
							line_segments) {
		std::vector<std::unique_ptr<geometry<D>>> ret;
		// basic O(n^2) line intersection
		for (auto iter1 = line_segments.begin(); iter1 != line_segments.end();
				 ++iter1)
			for (auto iter2 = std::next(iter1); iter2 != line_segments.end();
					 ++iter2) {
				const line_segment<D>& a = *iter1;
				const line_segment<D>& b = *iter2;
				std::vector<pt<D>> isect_pts = a.isect(b);
				for (const pt<D>& p : isect_pts)
					ret.push_back(std::make_unique<circle<D>>(p, isect_rad));
			}
		return ret;
	}

protected:
	virtual std::string name() { return "line segment intersection algorithm"; }

public:
	analysis_line_intersection() { this->print_tool_name(name()); }
};
