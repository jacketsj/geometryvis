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
	std::string s;
	double expiry;
	console_line() {}
	console_line(const std::string& s)
			: s(s), expiry(slGetTime() + text_lifetime) {}
	bool expired() { return slGetTime() > expiry; }
	col::col get_col() const {
		col::col ret = text_col;
		ret.a = (expiry - slGetTime()) / text_lifetime;
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

	void draw() {
		clean();
		std::vector<console_line> to_draw;
		for (auto iter = output.rbegin(); iter != output.rend(); ++iter)
			to_draw.push_back(*iter);
		int offset = 0;
		// int bottom = 0;
		col::col prev = col::cur_col;
		slSetFont(font, font_size);
		slSetTextAlign(SL_ALIGN_LEFT);
		for (const console_line& l : to_draw) {
			const std::string& s = l.s;
			// bottom = offset;
			offset += slGetTextHeight(s.c_str());
			l.get_col().set();
			slText(0.0f, offset, s.c_str());
		}
		prev.set();
	}
};
