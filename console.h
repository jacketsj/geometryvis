#pragma once

#include <algorithm>
#include <list>
#include <memory>
#include <sl.h>
#include <string>
#include <vector>

#include "col.h"

struct console_line {
	const double text_lifetime = 10.0f;
	const col::col text_col = col::white;
	const col::col bg_col = col::gray;
	std::string s;
	double expiry;
	console_line() {}
	console_line(const std::string& s)
			: s(s), expiry(slGetTime() + text_lifetime) {}
	bool expired() { return slGetTime() > expiry; }
	col::col get_col_bg() const {
		col::col ret = bg_col;
		ret.a = (expiry - slGetTime()) / text_lifetime;
		return ret;
	}
	col::col get_col() const {
		col::col ret = text_col;
		ret.a = (expiry - slGetTime()) / text_lifetime / 2.0f;
		return ret;
	}
};

class console {
private:
	const int font_size = 12;

	std::list<console_line> output;
	int font;
	console() { font = slLoadFont("ttf/white_rabbit.ttf"); }

	const static size_t MAX_LINES = 5;

	void clean() {
		while (!output.empty() &&
					 (output.size() > MAX_LINES || output.front().expired())) {
			output.pop_front();
		}
	}

	static void rect_with_coords(double x, double y, double width,
															 double height) {
		double center_x = x + width / 2.0f;
		double center_y = y + height / 2.0f;
		slRectangleFill(center_x, center_y, width, height);
	}

public:
	static console& get() {
		static console instance;
		return instance;
	}

	console(const console&) = delete;
	void operator=(const console&) = delete;

	void print(const std::string& s) {
		output.emplace_back(s);
		clean();
	}

	// TODO cleanup
	void draw() {
		clean();
		std::vector<console_line> to_draw;
		for (auto iter = output.rbegin(); iter != output.rend(); ++iter)
			to_draw.push_back(*iter);
		double margin_x = 4, margin_y = 4;
		double x, y, dx, dy, width, height;
		dx = 4;
		dy = 2;
		int offset = 0;
		int top = 0;
		col::col prev = col::cur_col;
		slSetFont(font, font_size);
		slSetTextAlign(SL_ALIGN_LEFT);
		for (const console_line& l : to_draw) {
			const std::string& s = l.s;
			top = offset;
			width = slGetTextWidth(s.c_str());
			height = slGetTextHeight(s.c_str());
			x = 0.0f + margin_x;
			y = double(offset) + margin_y;
			top += height;
			l.get_col_bg().set();
			rect_with_coords(x - dx / 2, y - dy, width + dx, height + dy / 2);
			l.get_col().set();
			slText(x, y, s.c_str());
			offset = top + 3;
		}
		prev.set();
	}
};
