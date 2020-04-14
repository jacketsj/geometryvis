#include <iostream>
#include <map>
#include <set>
#include <sl.h>
#include <tuple>
#include <vector>

const int W = 400, H = 400;

struct pt {
	int x, y;
	pt(int x, int y) : x(x), y(y) {}
	int norm2() const { return x * x + y * y; }
	pt operator-(const pt& oth) const { return pt(x - oth.x, y - oth.y); }
	friend int dist2(const pt& a, const pt& b) { return (a - b).norm2(); }
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

// map and set are slow for small values
struct mouse_watcher {
	std::map<char, bool> cur, prev; // just pressed
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
	bool down(char c) { return cur[c]; }
};

const int mb_left = SL_MOUSE_BUTTON_LEFT;
const int mb_right = SL_MOUSE_BUTTON_RIGHT;

int main(int args, char* argv[]) {
	// set up our window and a few resources we need
	slWindow(W, H, "Simple SIGIL Example", false);
	slSetFont(slLoadFont("ttf/white_rabbit.ttf"), 24);
	slSetTextAlign(SL_ALIGN_CENTER);
	int tex = slLoadTexture("png/glow.png");

	mouse_watcher keys;
	keys.watch(mb_left);
	keys.watch(mb_right);

	std::set<circle> circles;

	// int dt = 0;

	while (!slShouldClose() && !slGetKey(SL_KEY_ESCAPE)) {
		// get delta time
		// dt = slGetDeltaTime();

		// update keys
		keys.update();

		// get mouse x and y
		int mx = slGetMouseX();
		int my = slGetMouseY();
		pt mp(mx, my);

		// if left mouse button pressed, create a circle
		if (keys.pressed(mb_left)) {
			// if (keys.down(mb_left)) {
			// if (slGetKey(mb_left)) {
			circles.emplace(mx, my, 16);
		}

		// if right mouse button pressed, delete circles
		if (keys.down(mb_right)) {
			for (const auto& circ : circles) {
				if (circ.isect(mp)) {
					circles.erase(circ);
				}
			}
		}

		// background glow
		slSetForeColor(0.1, 0.9, 0.2, 0.4);

		slSprite(tex, 200, 240, 300, 200);

		// large text and fat line
		slSetForeColor(0.0, 0.8, 0.2, 1.0);
		slSetFontSize(24);
		slText(200, 250, "SIGIL:");
		slRectangleFill(200, 240, 100, 7);

		// smaller subtext
		slSetFontSize(14);
		slText(200, 220, "Sprites, text, sound, input, and more!");
		slLine(48, 210, 352, 210);

		// other lines
		slLine(100, 100, 360, 200);

		// draw circles
		for (auto circ : circles)
			circ.draw();

		slRender();
		// draw everything
	}
	// close the window and shut down SIGIL
	slClose();
	return 0;
}
