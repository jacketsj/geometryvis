#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "col.h"
#include "console.h"
#include "point.h"

template <typename D> class tool_point : public tool<D> {
private:
	pt<D> cur_mp;

	void place(canvas<D>& can, const pt<D>& p) {
		std::stringstream ss;
		ss << "created " << pt<D>::name() << ": " << p.to_string();
		console::get().print(ss.str());
		can.template push<pt<D>>(p);
	}

public:
	tool_point() { this->print_tool_name(pt<D>::name()); }
	virtual void l_click(canvas<D>& can) { place(can, cur_mp); }
	virtual void l_release(canvas<D>& can) {}
	virtual void r_click(canvas<D>& can) {}
	virtual void r_release(canvas<D>& can) {}
	virtual void draw() const {}
	virtual void update(canvas<D>& can, const pt<D>& mp) { cur_mp = mp; }
};
