#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "col.h"
#include "console.h"
#include "point.h"

template <typename T, typename D> class tool_2point : public tool<D> {
private:
	std::optional<pt<D>> cur;
	pt<D> cur_mp;

	void start(const pt<D>& p) { cur = std::make_optional(p); }
	void end(canvas<D>& can, const pt<D>& p) {
		if (cur) {
			std::stringstream ss;
			ss << "created " << T::name() << ": " << cur.value().to_string() << "--"
				 << p.to_string();
			console::get().print(ss.str());
			can.template push<T>(cur.value(), p);
			cur = std::nullopt;
		}
	}

public:
	tool_2point() : cur(std::nullopt) { this->print_tool_name(T::name()); }
	virtual void l_click(canvas<D>& can) { start(cur_mp); }
	virtual void l_release(canvas<D>& can) { end(can, cur_mp); }
	virtual void r_click(canvas<D>& can) {}
	virtual void r_release(canvas<D>& can) {}
	virtual void draw() const {
		if (cur) {
			col::col prev = col::cur_col;
			col::red.set();
			T cur_ls(cur.value(), cur_mp);
			cur_ls.draw();
			prev.set();
		}
	}
	virtual void update(canvas<D>& can, const pt<D>& mp) { cur_mp = mp; }
};
