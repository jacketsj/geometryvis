#include <map>
#include <memory>
#include <optional>
#include <set>
#include <sl.h>
#include <vector>

#include "button_handler.h"
#include "circle.h"
#include "col.h"
#include "console.h"
#include "geometry.h"
#include "line.h"
#include "point.h"
#include "tool.h"
#include "tool_circle.h"
#include "tool_handler.h"
#include "tool_line.h"

const double eps = 1e-7;

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

int main(int args, char* argv[]) {
	// dimensions of window
	dim D(1280, 720);

	// set up our window and a few resources we need
	slWindow(D.x, D.y, "Geometry Vis", false);
	slSetTextAlign(SL_ALIGN_CENTER);

	button_handler& mh = button_handler::get_mouse_handler();
	button_handler& kh = button_handler::get_key_handler();

	std::vector<std::unique_ptr<geometry>> geo_stack;

	tool_handler& th = tool_handler::get();
	std::unique_ptr<tool>& cur_tool = th.cur;

	col::blue.set();

	console& con = console::get();

	while (!slShouldClose() && !slGetKey(SL_KEY_ESCAPE)) {
		// set the default background col
		col::cur_col.set();

		// update the window dimensions
		D.update();

		// update the scaling accordingly
		D.render_transform();

		// update mouse handler and key handler
		mh.update();
		kh.update();

		// get mouse x and y
		int mx = slGetMouseX();
		int my = slGetMouseY();
		// world coordinate mouse point
		pt mp = D.reverse_transform(mx, my);

		// update tool handler
		th.update(mp, geo_stack);

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
