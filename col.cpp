#include "col.h"

#include "console.h"

namespace col {
void col::print() const {
	console::get().print("Colour: (" + std::to_string(r) + "," +
											 std::to_string(g) + "," + std::to_string(b) + "," +
											 std::to_string(a) + ")");
}
col white(1, 1, 1, 1);
col gray(0.5f, 0.5f, 0.5f, 0.5f);
col red(0.8, 0.15, 0.1, 1);
col green(0.1, 0.9, 0.2, 1);
col blue(0.1, 0.1, 0.85, 1);
col selected(1.0, 0.1, 0.1, 1);
} // namespace col
