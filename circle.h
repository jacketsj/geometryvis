#pragma once

#include <cmath>
#include <string>
#include <vector>

#include "basic_rect.h"
#include "geometry.h"
#include "line.h"
#include "point.h"
#include "properties.h"
#include "render_adapter.h"

template <typename D> class circle : public geometry<D> {
private:
	bool isect(const pt<D>& p0) const { return dist2(p, p0) <= r2; }
	bool isect(int x0, int y0) const { return isect(pt<D>(x0, y0)); }

protected:
	virtual circle* clone_impl() const { return new circle(*this); }

public:
	pt<D> p;
	D r2;
	circle(D x, D y, D r2) : p(x, y), r2(r2) {}
	circle(const pt<D>& p, D r2) : p(p), r2(r2) {}
	circle(const pt<D>& p, const pt<D>& pr) : p(p), r2(dist2(pr, p)) {}
	static std::string name() { return "circle"; }
	virtual void draw() const;
	virtual std::vector<D> comp_list() const {
		std::vector<D> ret = p.comp_list();
		ret.push_back(r2);
		return ret;
	}
	D rad() const { return sqrt(r2); }
	line_segment<D> tangent_line(const circle& origin, bool cw = false) const {
		// based on UBC Code Archive code
		auto polar = [](const D& rho, const D& theta) {
			return pt<D>(rho * cos(theta), rho * sin(theta));
		};
		auto circle_tangent = [&](D r1, D r2, D d,
															int k) { // use the fcn below
			D dr = (k & 2) ? (-r1 - r2) : (r2 - r1);
			D t = asin(dr / d);
			pt<D> p1 = polar(r1, M_PI / 2 + t), p2 = polar(r2, M_PI / 2 + t);
			if (k & 2)
				p2 *= -1;
			p2 += pt<D>(d, 0);
			if (k & 1) {
				p1 = pt<D>(p1.x, -p1.y);
				p2 = pt<D>(p2.x, -p2.y);
			}
			return std::make_pair(p1, p2);
		};
		// tested 2008wf conveyor; tangent of 2 circles; CAUTION: INTERSECTION IS
		// BAD k=0 top-top, k=1 bot-bot, k=2 top-bot, k=3 bot-top. Also works for
		// points.
		auto circle_tangent2 = [&](pt<D> p1, D r1, pt<D> p2, D r2, int k) {
			// translate/rotate so c1 at (0,0), c2 at x-axis
			pt<D> d = p2 - p1;
			std::pair<pt<D>, pt<D>> p = circle_tangent(r1, r2, d.norm(), k);
			d = d.normalize();
			p.first *= d;
			p.second *= d;
			p.first += p1;
			p.second += p1;
			return p;
		};
		auto res = circle_tangent2(origin.p, origin.rad(), p, rad(), 0);
		return line_segment<D>(res.first, res.second);
	}

	pt<D> tangent_line_dif(const circle& origin, bool cw = false) const {
		return tangent_line(origin, cw).dif();
	}

	bool inside(const circle& oth, D eps) const {
		return dist(p, oth.p) < oth.rad() - rad() - eps;
	}

	virtual basic_rect<D> bounding_box() const {
		return (rect<D>(p, pt<D>(2 * sqrt(r2), 2 * sqrt(r2)))).to_basic();
	}

	virtual void translate(const basic_pt<D>& delta) { p += delta; }
};
