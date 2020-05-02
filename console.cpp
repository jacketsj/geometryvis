#include "console.h"

#include "render_adapter.h"

bool console_line::expired() const {
	return render_adapter::get_time() > expiry;
}
col::col console_line::get_col_bg() const {
	col::col ret = bg_col;
	ret.a = std::min(expiry - render_adapter::get_time(), text_lifetime) /
					text_lifetime;
	return ret;
}
col::col console_line::get_col() const {
	col::col ret = text_col;
	ret.a = std::min(expiry - render_adapter::get_time(), text_lifetime) /
					text_lifetime / 2.0f;
	return ret;
}
console_line::console_line(const std::string& s, bool expires)
		: s(s), expiry(expires ? render_adapter::get_time() + text_lifetime
													 : std::numeric_limits<double>::infinity()) {}

console::console() {
	display_history = false;
	render_adapter::init_font();
}
void console::clean() {
	while (!output.empty() &&
				 (output.size() > MAX_LINES || output.front().expired())) {
		output.pop_front();
	}
}
void console::print(const std::string& s) {
	output.emplace_back(s);
	history.emplace_back(s, false);
	clean();
}
void console::rect_with_coords(double x, double y, double width, double height,
															 const properties& prop) {
	double center_x = x + width / 2.0f;
	double center_y = y + height / 2.0f;
	render_adapter::draw_rect(pt(center_x, center_y), pt(width, height), prop);
}
// TODO cleanup
void console::draw() {
	clean();
	std::vector<console_line> to_draw;
	std::list<console_line>& current_vis = display_history ? history : output;
	for (auto iter = current_vis.rbegin();
			 iter != current_vis.rend() && to_draw.size() <= MAX_LINES_HISTORY;
			 ++iter)
		to_draw.push_back(*iter);
	double margin_x = 4, margin_y = 4;
	double x, y, dx, dy, width, height;
	dx = 4;
	dy = 2;
	int offset = 0;
	int top = 0;
	for (const console_line& l : to_draw) {
		const std::string& s = l.s;
		top = offset;
		pt size = render_adapter::get_text_dimensions(s);
		width = size.x;
		height = size.y;
		x = 0.0f + margin_x;
		y = double(offset) + margin_y;
		top += height;
		properties prop;
		prop.c = l.get_col_bg();
		prop.fill = true;
		rect_with_coords(x - dx / 2, y - dy, width + dx, height + dy / 2, prop);
		prop.c = l.get_col();
		prop.font_size = 12;
		render_adapter::draw_text(pt<double>(x, y), s, prop);
		offset = top + 3;
	}
}
