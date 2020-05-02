#include <map>
#include <memory>
#include <optional>
#include <set>
#include <sl.h>
#include <vector>

#include "button_handler.h"
#include "canvas.h"
#include "circle.h"
#include "col.h"
#include "console.h"
#include "geometry.h"
#include "line.h"
#include "point.h"
#include "tool.h"
#include "tool_handler.h"

const double eps = 1e-7;

struct dimensions {
	// internal dimensions
	int x, y;
	double scale_x, scale_y;
	dimensions(int x, int y) : x(x), y(y), scale_x(1), scale_y(1) {}
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
	pt<double> reverse_transform(double x0, double y0) {
		// return pt<double>(round(x0 / scale_x), round(y0 / scale_y));
		return pt<double>(x0 / scale_x, y0 / scale_y);
	}
};

int main(int args, char* argv[]) {
	// dimensions of window
	dimensions dim(1280, 720);

	// set up our window and a few resources we need
	slWindow(dim.x, dim.y, "Geometry Vis", false);
	slSetTextAlign(SL_ALIGN_CENTER);

	button_handler& mh = button_handler::get_mouse_handler();
	button_handler& kh = button_handler::get_key_handler();

	canvas<double> can;

	tool_handler& th = tool_handler::get();

	console& con = console::get();

	while (!slShouldClose() && !slGetKey(SL_KEY_ESCAPE)) {
		// update the window dimensions
		dim.update();

		// update the scaling accordingly
		dim.render_transform();

		// update mouse handler and key handler
		mh.update();
		kh.update();

		// get mouse x and y
		double mx = slGetMouseX();
		double my = slGetMouseY();
		// world coordinate mouse point
		pt<double> mp = dim.reverse_transform(mx, my);

		// update tool handler (which also updates tool)
		th.update(can, mp);

		// draw all geometry
		can.draw();

		// draw tool
		th.draw();

		// draw console
		dim.render_notransform();
		con.draw();

		// draw everything
		slRender();
	}
	// close the window and shut down SIGIL
	slClose();
	return 0;
}
