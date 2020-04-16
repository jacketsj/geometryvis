#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "col.h"
#include "console.h"
#include "point.h"

class tool_circle : public tool {
private:
	void start(const pt& p) { cur = std::make_optional(p); }
	void end(std::vector<std::unique_ptr<geometry>>& geo_stack, const pt& p) {
		if (cur) {
			std::stringstream ss;
			ss << "created circle: (" << cur.value().to_string() << ")--"
				 << p.to_string();
			console::get().print(ss.str());
			geo_stack.push_back(
					std::make_unique<circle>(cur.value(), dist2(cur.value(), p)));
			cur = std::nullopt;
		}
	}
	std::optional<pt> cur;

public:
	tool_circle() : cur(std::nullopt) { print_tool_name("Circle"); }
	virtual void l_click(std::vector<std::unique_ptr<geometry>>& geo_stack,
											 pt mp) {
		start(mp);
	}
	virtual void l_release(std::vector<std::unique_ptr<geometry>>& geo_stack,
												 pt mp) {
		end(geo_stack, mp);
	}
	virtual void r_click(std::vector<std::unique_ptr<geometry>>& geo_stack,
											 pt mp) {}
	virtual void r_release(std::vector<std::unique_ptr<geometry>>& geo_stack,
												 pt mp) {}
	virtual void draw(pt mp) const {
		if (cur) {
			col::col prev = col::cur_col;
			col::red.set();
			circle cur_circ(cur.value(), dist2(cur.value(), mp));
			cur_circ.draw();
			prev.set();
		}
	}
};
