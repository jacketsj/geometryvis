#pragma once

#include <vector>

#include "geometry.h"
#include "point.h"
#include "properties.h"
#include "rect.h"

template <typename D> class line_segment : public geometry<D> {
protected:
	virtual line_segment* clone_impl() const { return new line_segment(*this); }

public:
	pt<D> p1, p2;
	line_segment(pt<D> p1, pt<D> p2) : p1(p1), p2(p2) {}
	static std::string name() { return "line segment"; }
	virtual void draw() const;
	virtual std::vector<D> comp_list() const {
		std::vector<D> ret = p1.comp_list();
		std::vector<D> p2_cl = p2.comp_list();
		ret.insert(ret.end(), p2_cl.begin(), p2_cl.end());
		return ret;
	}
	std::vector<pt<D>> isect(const line_segment<D>& oth) const;

	pt<D> dif() const { return p2 - p1; }

	virtual basic_rect<D> bounding_box() const {
		return (rect<D>(p1, p2)).to_basic();
	}

	virtual void translate(const basic_pt<D>& delta) {
		p1 += delta;
		p2 += delta;
	}
};
