#pragma once

#include <sl.h>

namespace col {
struct col {
	double r, g, b, a;
	col(double r, double g, double b, double a) : r(r), g(g), b(b), a(a) {}
	void set() const;
};
col white(1, 1, 1, 1);
col gray(0.5f, 0.5f, 0.5f, 0.5f);
col red(0.8, 0.15, 0.1, 1);
col green(0.1, 0.9, 0.2, 1);
col blue(0.1, 0.1, 0.85, 1);
col cur_col = white;
void col::set() const {
	cur_col = *this;
	slSetForeColor(r, g, b, a);
}
}; // namespace col
