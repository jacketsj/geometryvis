#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <sl.h>
#include <tuple>
#include <vector>

const double eps = 1e-7;

namespace col {
struct col {
	double r, g, b, a;
	col(double r, double g, double b, double a) : r(r), g(g), b(b), a(a) {}
	void set() const;
};
col white(1, 1, 1, 1);
col red(0.8, 0.15, 0.1, 1);
col green(0.1, 0.9, 0.2, 1);
col blue(0.1, 0.1, 0.85, 1);
col cur_col = white;
void col::set() const {
	cur_col = *this;
	slSetForeColor(r, g, b, a);
}
}; // namespace col

struct pt {
	int x, y;
	pt(int x, int y) : x(x), y(y) {}
	int norm2() const { return x * x + y * y; }
	pt operator-(const pt& oth) const { return pt(x - oth.x, y - oth.y); }
	friend int dist2(const pt& a, const pt& b) { return (a - b).norm2(); }
	bool operator<(const pt& oth) const {
		return std::tie(x, y) < std::tie(oth.x, oth.y);
	}
	void print() const { std::cout << "(" << x << ',' << y << ")"; }
};

struct circle {
	pt p;
	int r;
	circle(int x, int y, int r) : p(x, y), r(r) {}
	void draw() const { slCircleFill(p.x, p.y, r, r); }
	bool isect(const pt& p0) const { return dist2(p, p0) <= r * r; }
	bool isect(int x0, int y0) const { return isect(pt(x0, y0)); }
	bool operator<(const circle& oth) const {
		return std::tie(p.x, p.y, r) < std::tie(oth.p.x, oth.p.y, oth.r);
	}
};

struct line_segment {
	pt p1, p2;
	int r; // thickness
	line_segment(pt p1, pt p2) : p1(p1), p2(p2), r(1) {}
	line_segment(pt p1, pt p2, int r) : p1(p1), p2(p2), r(r) {}
	void draw() const { slLine(p1.x, p1.y, p2.x, p2.y); }
	bool operator<(const line_segment& oth) const {
		return std::tie(p1, p2) < std::tie(oth.p1, oth.p2);
	}
};

struct line_drawer {
	std::optional<pt> cur;
	std::vector<line_segment> segs;
	line_drawer() : cur(std::nullopt) {}
	void start(const pt& p) { cur = std::make_optional(p); }
	void end(const pt& p) {
		if (cur) {
			std::cout << "created segment: ";
			cur.value().print();
			std::cout << "--";
			p.print();
			std::cout << std::endl;
			segs.emplace_back(cur.value(), p);
			cur = std::nullopt;
		}
	}
	void draw(const pt& mouse) {
		for (const auto& ls : segs)
			ls.draw();
		if (cur) {
			col::col prev = col::cur_col;
			col::red.set();
			line_segment cur_ls(cur.value(), mouse);
			cur_ls.draw();
			prev.set();
		}
	}
};

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
	static int round(double d) { return int(d + 0.5 + eps); }
	pt reverse_transform(double x0, double y0) {
		// return pt(round(x0 / scale_x), round(y0 / scale_y));
		return pt(x0 / scale_x, y0 / scale_y);
	}
};

int main(int args, char* argv[]) {
	// dimensions of window
	dim D(640, 480);

	// set up our window and a few resources we need
	slWindow(D.x, D.y, "Geometry Vis", false);
	slSetTextAlign(SL_ALIGN_CENTER);

	mouse_watcher mw;
	mw.watch(mb_left);
	mw.watch(mb_right);

	line_drawer ld;

	col::blue.set();

	// int dt = 0;

	while (!slShouldClose() && !slGetKey(SL_KEY_ESCAPE)) {
		// set the default background col
		col::cur_col.set();

		// update the window dimensions
		D.update();

		// update the scaling accordingly
		D.render_transform();

		// get delta time
		// dt = slGetDeltaTime();

		// update mouse watcher
		mw.update();

		// get mouse x and y
		int mx = slGetMouseX();
		int my = slGetMouseY();
		// world coordinate mouse point
		pt mp = D.reverse_transform(mx, my);

		// if left mouse button pressed, start line segment
		if (mw.pressed(mb_left)) {
			ld.start(mp);
		}

		// if left mouse button released, end line segment
		if (mw.released(mb_left)) {
			ld.end(mp);
		}

		// draw all line segments
		ld.draw(mp);

		// draw everything
		slRender();
	}
	// close the window and shut down SIGIL
	slClose();
	return 0;
}
