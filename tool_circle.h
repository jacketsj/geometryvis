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
	std::optional<pt> cur;
	pt cur_mp;

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

public:
	tool_circle() : cur(std::nullopt) { print_tool_name("Circle"); }
	virtual void l_click(std::vector<std::unique_ptr<geometry>>& geo_stack) {
		start(cur_mp);
	}
	virtual void l_release(std::vector<std::unique_ptr<geometry>>& geo_stack) {
		end(geo_stack, cur_mp);
	}
	virtual void r_click(std::vector<std::unique_ptr<geometry>>& geo_stack) {}
	virtual void r_release(std::vector<std::unique_ptr<geometry>>& geo_stack) {}
	virtual void draw() const {
		if (cur) {
			col::col prev = col::cur_col;
			col::red.set();
			circle cur_circ(cur.value(), dist2(cur.value(), cur_mp));
			cur_circ.draw();
			prev.set();
		}
	}
	virtual void update(const pt& mp) { cur_mp = mp; }
};
