#pragma once

#include "col.h"

class properties {
public:
	col::col c;
	double size;
	double font_size;
	bool fill;

	properties(col::col c, double size, double fontsize, bool fill)
			: c(c), size(size), font_size(fontsize), fill(fill) {}
	properties() { use_default_properties(); }
	void use_default_properties();
};

extern properties default_properties;
