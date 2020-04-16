#pragma once

#include <memory>
#include <string>
#include <vector>

#include "point.h"

class tool {
protected:
	virtual void print_tool_name(const std::string& tool_name) {
		console::get().print(std::string("Tool: ") + tool_name);
	}
	virtual void print_tool_name(const char* c_str) {
		print_tool_name(std::string(c_str));
	}

public:
	virtual void l_click(std::vector<std::unique_ptr<geometry>>& geo_stack) = 0;
	virtual void l_release(std::vector<std::unique_ptr<geometry>>& geo_stack) = 0;
	virtual void r_click(std::vector<std::unique_ptr<geometry>>& geo_stack) = 0;
	virtual void r_release(std::vector<std::unique_ptr<geometry>>& geo_stack) = 0;
	virtual void draw() const = 0;
	virtual void update(const pt& mp) = 0;
};
