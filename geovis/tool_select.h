#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "col.h"
#include "console.h"
#include "point.h"

template <typename D> class tool_select : public tool<D> {
private:
	pt<D> cur_mp;

public:
	tool_select() { this->print_tool_name("select"); }
	virtual void l_click(canvas<D>& can) { can.select(cur_mp); }
	virtual void l_release(canvas<D>& can) {}
	virtual void r_click(canvas<D>& can) { can.deselect_all(); }
	virtual void r_release(canvas<D>& can) {}
	virtual void draw() const {}
	virtual void update(canvas<D>& can, const pt<D>& mp) { cur_mp = mp; }
};
