#pragma once

#include <memory>
#include <vector>

#include "col.h"
#include "point.h"

class tool_line : public tool {
private:
	void start(const pt& p) { cur = std::make_optional(p); }
	void end(std::vector<std::unique_ptr<geometry>>& geo_stack, const pt& p) {
		if (cur) {
			std::cout << "created segment: ";
			cur.value().print();
			std::cout << "--";
			p.print();
			std::cout << std::endl;
			geo_stack.push_back(std::make_unique<line_segment>(cur.value(), p));
			cur = std::nullopt;
		}
	}
	std::optional<pt> cur;

public:
	tool_line() : cur(std::nullopt) {}
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
			line_segment cur_ls(cur.value(), mp);
			cur_ls.draw();
			prev.set();
		}
	}
};
