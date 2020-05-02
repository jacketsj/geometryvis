#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "col.h"
#include "console.h"
#include "point.h"

template <typename D> class tool_translate : public tool<D> {
private:
	std::optional<pt<D>> start;
	pt<D> cur_mp;

public:
	tool_translate() { this->print_tool_name("translate"); }
	virtual void l_click(canvas<D>& can) { start = std::make_optional(cur_mp); }
	virtual void l_release(canvas<D>& can) {
		pt<D> delta = cur_mp - start.value();
		std::vector<std::reference_wrapper<geometry<D>>> selected =
				can.get_selected();
		for (geometry<D>& cur : selected) {
			cur.translate(delta.to_basic());
		}
		start = std::nullopt;
	}
	virtual void r_click(canvas<D>& can) {}
	virtual void r_release(canvas<D>& can) {}
	virtual void draw() const {}
	virtual void update(canvas<D>& can, const pt<D>& mp) {
		cur_mp = mp;
		if (start)
			can.draw_selected(cur_mp - start.value());
	}
};
