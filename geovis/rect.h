#pragma once

#include <cmath>
#include <sstream>
#include <string>
#include <vector>

#include "basic_rect.h"
#include "geometry.h"
#include "point.h"
#include "properties.h"

template <typename D> class rect : public geometry<D> {
protected:
	virtual rect* clone_impl() const { return new rect(*this); }

public:
	pt<D> p, sz;
	rect(D x, D y, D w, D h) : p(x, y), sz(w, h) {}
	rect() : p(), sz() {}
	rect(const basic_rect<D>& br) : p(br.x, br.y), sz(br.w, br.h) {}
	rect(const pt<D>& a, const pt<D>& b) {
		p = pt<D>((a.x + b.x) / 2, (a.y + b.y) / 2);
		sz = pt<D>(abs(b.x - a.x), abs(b.y - a.y));
	}
	static std::string name() { return "rectangle"; }

	virtual std::vector<D> comp_list() const {
		std::vector<D> ret = p.comp_list();
		std::vector<D> sz_cl = sz.comp_list();
		ret.insert(ret.end(), sz_cl.begin(), sz_cl.end());
		return ret;
	}
	virtual void draw() const;

	bool contains(const pt<D>& a) {
		return abs(a.x - p.x) <= sz.x / 2 && abs(a.y - p.y) <= sz.y / 2;
	}

	basic_rect<D> to_basic() const { return basic_rect<D>(p.x, p.y, sz.x, sz.y); }

	virtual basic_rect<D> bounding_box() const { return to_basic(); }

	rect<D> pad(D amt) {
		rect<D> ret = *this;
		ret.sz.x += amt;
		ret.sz.y += amt;
		return ret;
	}

	virtual void translate(const basic_pt<D>& delta) { p += delta; }
};
