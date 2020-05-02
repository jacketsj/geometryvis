#pragma once

#include <algorithm>
#include <limits>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "col.h"
#include "properties.h"

struct console_line {
	const double text_lifetime = 10.0f;
	const col::col text_col = col::white;
	const col::col bg_col = col::gray;
	std::string s;
	double expiry;
	console_line() {}
	console_line(const std::string& s, bool expires = true);
	bool expired() const;
	col::col get_col_bg() const;
	col::col get_col() const;
};

class console {
private:
	const int font_size = 12;

	std::list<console_line> output;
	std::list<console_line> history;
	bool display_history;
	int font;
	console();

	const static size_t MAX_LINES = 5;
	const static size_t MAX_LINES_HISTORY = 100;

	void clean();

	static void rect_with_coords(double x, double y, double width, double height,
															 const properties& prop);

public:
	static console& get() {
		static console instance;
		return instance;
	}

	console(const console&) = delete;
	void operator=(const console&) = delete;

	void print(const std::string& s);

	void set_history_display(bool show) { display_history = show; }

	void draw();
};
