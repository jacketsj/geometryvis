#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "col.h"
#include "console.h"
#include "point.h"

template <typename T> class tool_2point : public tool {
private:
	std::optional<pt> cur;
	pt cur_mp;

	void start(const pt& p) { cur = std::make_optional(p); }
	void end(canvas& can, const pt& p) {
		if (cur) {
			std::stringstream ss;
			ss << "created " << T::name() << ": " << cur.value().to_string() << "--"
				 << p.to_string();
			console::get().print(ss.str());
			can.push<T>(cur.value(), p);
			cur = std::nullopt;
		}
	}

public:
	tool_2point() : cur(std::nullopt) { print_tool_name(T::name()); }
	virtual void l_click(canvas& can) { start(cur_mp); }
	virtual void l_release(canvas& can) { end(can, cur_mp); }
	virtual void r_click(canvas& can) {}
	virtual void r_release(canvas& can) {}
	virtual void draw() const {
		if (cur) {
			col::col prev = col::cur_col;
			col::red.set();
			T cur_ls(cur.value(), cur_mp);
			cur_ls.draw();
			prev.set();
		}
	}
	virtual void update(canvas& can, const pt& mp) { cur_mp = mp; }
};
