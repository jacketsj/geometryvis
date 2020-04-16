#include <map>
#include <memory>
#include <optional>
#include <set>
#include <sl.h>
#include <tuple>
#include <vector>

#include "circle.h"
#include "col.h"
#include "console.h"
#include "geometry.h"
#include "line.h"
#include "point.h"
#include "tool.h"
#include "tool_circle.h"
#include "tool_line.h"

const double eps = 1e-7;

// map and set are slow for small values
struct mouse_watcher {
	std::map<char, bool> cur, prev; // just pressed std::set<char> watching;
	std::set<char> watching;
	void watch(char c) { watching.insert(c); }
	void unwatch(char c) { watching.erase(c); }
	void update() {
		prev = cur;
		for (char c : watching) {
			cur[c] = slGetMouseButton(c);
		}
	}
	bool pressed(char c) { return cur[c] && !prev[c]; }
	bool released(char c) { return !cur[c] && prev[c]; }
	bool down(char c) { return cur[c]; }
};

const int mb_left = SL_MOUSE_BUTTON_LEFT;
const int mb_right = SL_MOUSE_BUTTON_RIGHT;

struct dim {
	// internal dimensions
	int x, y;
	double scale_x, scale_y;
	dim(int x, int y) : x(x), y(y), scale_x(1), scale_y(1) {}
	void update() {
		int x0, y0;
		slGetWindowSize(&x0, &y0);
		scale_x = double(x0) / double(x);
		scale_y = double(y0) / double(y);
	}
	void render_transform() {
		slIdentity();
		slScale(scale_x, scale_y);
	};
	void render_notransform() { slIdentity(); };
	static int round(double d) { return int(d + 0.5 + eps); }
	pt reverse_transform(double x0, double y0) {
		// return pt(round(x0 / scale_x), round(y0 / scale_y));
		return pt(x0 / scale_x, y0 / scale_y);
	}
};

namespace tool_handler {
std::unique_ptr<tool> init() { return std::make_unique<tool_circle>(); }
} // namespace tool_handler

int main(int args, char* argv[]) {
	// dimensions of window
	dim D(1280, 720);

	// set up our window and a few resources we need
	slWindow(D.x, D.y, "Geometry Vis", false);
	slSetTextAlign(SL_ALIGN_CENTER);

	mouse_watcher mw;
	mw.watch(mb_left);
	mw.watch(mb_right);

	std::vector<std::unique_ptr<geometry>> geo_stack;
	std::unique_ptr<tool> cur_tool = tool_handler::init();

	col::blue.set();

	console& con = console::get();

	while (!slShouldClose() && !slGetKey(SL_KEY_ESCAPE)) {
		// set the default background col
		col::cur_col.set();

		// update the window dimensions
		D.update();

		// update the scaling accordingly
		D.render_transform();

		// update mouse watcher
		mw.update();

		// get mouse x and y
		int mx = slGetMouseX();
		int my = slGetMouseY();
		// world coordinate mouse point
		pt mp = D.reverse_transform(mx, my);

		// if mouse button released, pass it on to tool
		if (mw.released(mb_left)) {
			cur_tool->l_release(geo_stack, mp);
		}
		if (mw.released(mb_right)) {
			cur_tool->r_release(geo_stack, mp);
		}

		// if mouse button pressed, pass it on to tool
		if (mw.pressed(mb_left)) {
			cur_tool->l_click(geo_stack, mp);
		}
		if (mw.pressed(mb_right)) {
			cur_tool->r_click(geo_stack, mp);
		}

		// draw all geometry
		for (auto& geo_ptr : geo_stack)
			geo_ptr->draw();

		// draw tool
		cur_tool->draw(mp);

		// draw console
		D.render_notransform();
		con.draw();

		// draw everything
		slRender();
	}
	// close the window and shut down SIGIL
	slClose();
	return 0;
}
