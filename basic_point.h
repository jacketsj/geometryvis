#pragma once

template <typename D0> class basic_pt {
public:
	D0 x, y;
	basic_pt(D0 x, D0 y) : x(x), y(y) {}
	basic_pt() : x(0), y(0) {}
};
