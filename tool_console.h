#pragma once

#include <memory>
#include <string>
#include <vector>

#include "canvas.h"
#include "console.h"
#include "point.h"

template <typename D> class tool_console : public tool<D> {
public:
	tool_console() {
		console::get().set_history_display(true);
		this->print_tool_name("console");
	}
	~tool_console() { console::get().set_history_display(false); }
	virtual void l_click(canvas<D>& can) {}
	virtual void l_release(canvas<D>& can) {}
	virtual void r_click(canvas<D>& can) {}
	virtual void r_release(canvas<D>& can) {}
	virtual void draw() const {}
	virtual void update(canvas<D>& c, const pt<D>& mp) {}
};
