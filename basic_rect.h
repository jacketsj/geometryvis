#pragma once

template <typename D0> class basic_rect {
public:
	D0 x, y, w, h;
	basic_rect(D0 x, D0 y, D0 w, D0 h) : x(x), y(y), w(w), h(h) {}
	basic_rect() : x(0), y(0), w(0), h(0) {}
};
