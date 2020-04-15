#pragma once

#include <memory>
#include <vector>

#include "point.h"

class tool {
public:
	virtual void l_click(std::vector<std::unique_ptr<geometry>>& geo_stack,
											 pt mp) = 0;
	virtual void l_release(std::vector<std::unique_ptr<geometry>>& geo_stack,
												 pt mp) = 0;
	virtual void r_click(std::vector<std::unique_ptr<geometry>>& geo_stack,
											 pt mp) = 0;
	virtual void r_release(std::vector<std::unique_ptr<geometry>>& geo_stack,
												 pt mp) = 0;
	virtual void draw(pt mp) const = 0;
};
