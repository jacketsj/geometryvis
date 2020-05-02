#pragma once

#include <assert.h>
#include <memory>

#include "analysis_convex_hull.h"
#include "analysis_convex_hull_circles.h"
#include "analysis_line_intersection.h"
#include "button_handler.h"
#include "canvas.h"
#include "circle.h"
#include "console.h"
#include "line.h"
#include "point.h"
#include "tool.h"
#include "tool_2point.h"
#include "tool_console.h"
#include "tool_point.h"
#include "tool_select.h"
#include "tool_translate.h"

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
		for (char i = '0'; i <= '9'; ++i)
			kh.watch(i);
		for (char i = 'A'; i <= 'Z'; ++i)
			kh.watch(i);
		kh.watch(tilde);
		mh.watch(mb_left);
		mh.watch(mb_right);
		kh.watch(undo);
		kh.watch(ctrl);
		kh.watch(shift);
		kh.watch(del);
	}

	const static int mb_left = SL_MOUSE_BUTTON_LEFT;
	const static int mb_right = SL_MOUSE_BUTTON_RIGHT;
	const static int tilde = '`';
	const static int undo = 'Z';
	const static int ctrl = SL_KEY_LEFT_CONTROL;
	const static int shift = SL_KEY_LEFT_SHIFT;
	const static int del = SL_KEY_DELETE;

	std::vector<col::col> col_chooser = {col::blue, col::red, col::green,
																			 col::white, col::gray};

public:
	static tool_handler& get() {
		static tool_handler instance;
		return instance;
	}

	tool_handler(const tool_handler&) = delete;
	void operator=(const tool_handler&) = delete;

	void update(canvas<D>& can, const pt<D>& mp) {
		// handle undo/redo

		if (kh.down(ctrl) && kh.down(shift) && kh.pressed(undo))
			can.redo();
		else if (kh.down(ctrl) && kh.pressed(undo))
			can.undo();
		else if (kh.pressed(del))
			can.delete_selected();
		// handle tool-changing events
		if (kh.pressed(tilde))
			cur = std::make_unique<tool_console<D>>();
		else if (kh.pressed('S'))
			cur = std::make_unique<tool_select<D>>();
		else if (kh.pressed('T'))
			cur = std::make_unique<tool_translate<D>>();
		else if (kh.pressed('Q'))
			cur = std::make_unique<tool_point<D>>();
		else if (kh.pressed('W'))
			cur = std::make_unique<tool_2point<line_segment<D>, D>>();
		else if (kh.pressed('E'))
			cur = std::make_unique<tool_2point<circle<D>, D>>();
		else if (kh.pressed('U'))
			cur = std::make_unique<analysis_convex_hull<D>>();
		else if (kh.pressed('I'))
			cur = std::make_unique<analysis_line_intersection<D>>();
		else if (kh.pressed('O'))
			cur = std::make_unique<analysis_convex_hull_circles<D>>();
		assert(col_chooser.size() <= 9);
		for (size_t button = 0; button <= col_chooser.size(); ++button) {
			if (kh.pressed(button + '1')) {
				default_properties.c = col_chooser[button];
				default_properties.c.print();
			}
		}

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
