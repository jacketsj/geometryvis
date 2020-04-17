#pragma once

#include <memory>

#include "button_handler.h"
#include "circle.h"
#include "line.h"
#include "point.h"
#include "tool.h"
#include "tool_2point.h"

class tool_handler {
public:
	std::unique_ptr<tool> cur;

private:
	button_handler& mh;
	button_handler& kh;

	tool_handler()
			: cur(std::make_unique<tool_circle>()),
				mh(button_handler::get_mouse_handler()),
				kh(button_handler::get_key_handler()) {
		for (char i = 0; i <= 9; ++i)
			kh.watch('0' + i);
		mh.watch(mb_left);
		mh.watch(mb_right);
	}

	const static int mb_left = SL_MOUSE_BUTTON_LEFT;
	const static int mb_right = SL_MOUSE_BUTTON_RIGHT;

public:
	static tool_handler& get() {
		static tool_handler instance;
		return instance;
	}

	tool_handler(const tool_handler&) = delete;
	void operator=(const tool_handler&) = delete;

	void update(const pt& mp, std::vector<std::unique_ptr<geometry>>& geo_stack) {
		// handle tool-changing events
		if (kh.pressed('1'))
			cur = std::make_unique<tool_2point<line_segment>>();
		else if (kh.pressed('2'))
			cur = std::make_unique<tool_2point<circle>>();

		// update the tool
		cur->update(mp);

		// handle events to the tools
		// if mouse button pressed, pass it on to tool
		if (mh.pressed(mb_left)) {
			cur->l_click(geo_stack);
		}
		if (mh.pressed(mb_right)) {
			cur->r_click(geo_stack);
		}

		// if mouse button released, pass it on to tool
		if (mh.released(mb_left)) {
			cur->l_release(geo_stack);
		}
		if (mh.released(mb_right)) {
			cur->r_release(geo_stack);
		}
	}
};
