#pragma once

#include <memory>

#include "analysis_convex_hull.h"
#include "analysis_line_intersection.h"
#include "button_handler.h"
#include "canvas.h"
#include "circle.h"
#include "line.h"
#include "point.h"
#include "tool.h"
#include "tool_2point.h"
#include "tool_console.h"
#include "tool_point.h"

class tool_handler {
	typedef double D;

public:
	std::unique_ptr<tool<D>> cur;

private:
	button_handler& mh;
	button_handler& kh;

	tool_handler()
			: cur(std::make_unique<tool_point<D>>()),
				mh(button_handler::get_mouse_handler()),
				kh(button_handler::get_key_handler()) {
		for (char i = 0; i <= 9; ++i)
			kh.watch('0' + i);
		kh.watch(tilde);
		mh.watch(mb_left);
		mh.watch(mb_right);
	}

	const static int mb_left = SL_MOUSE_BUTTON_LEFT;
	const static int mb_right = SL_MOUSE_BUTTON_RIGHT;
	const static int tilde = '`';

public:
	static tool_handler& get() {
		static tool_handler instance;
		return instance;
	}

	tool_handler(const tool_handler&) = delete;
	void operator=(const tool_handler&) = delete;

	void update(canvas<D>& can, const pt<D>& mp) {
		// handle tool-changing events
		if (kh.pressed(tilde))
			cur = std::make_unique<tool_console<D>>();
		else if (kh.pressed('1'))
			cur = std::make_unique<tool_point<D>>();
		else if (kh.pressed('2'))
			cur = std::make_unique<tool_2point<line_segment<D>, D>>();
		else if (kh.pressed('3'))
			cur = std::make_unique<tool_2point<circle<D>, D>>();
		else if (kh.pressed('4'))
			cur = std::make_unique<analysis_convex_hull<D>>();
		else if (kh.pressed('5'))
			cur = std::make_unique<analysis_line_intersection<D>>();

		// update the tool
		cur->update(can, mp);

		// handle events to the tools
		// if mouse button pressed, pass it on to tool
		if (mh.pressed(mb_left)) {
			cur->l_click(can);
		}
		if (mh.pressed(mb_right)) {
			cur->r_click(can);
		}

		// if mouse button released, pass it on to tool
		if (mh.released(mb_left)) {
			cur->l_release(can);
		}
		if (mh.released(mb_right)) {
			cur->r_release(can);
		}
	}

	void draw() { cur->draw(); }
};
