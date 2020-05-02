#ifndef COL_H
#define COL_H

namespace col {
struct col {
	double r, g, b, a;
	col() : r(0), g(0), b(0), a(0) {}
	col(double r, double g, double b, double a) : r(r), g(g), b(b), a(a) {}
	void print() const;
};
extern col white;
extern col gray;
extern col red;
extern col green;
extern col blue;
}; // namespace col

#endif
