#include "properties.h"

void properties::use_default_properties() { *this = default_properties; }

properties default_properties = properties(col::blue, 4, 12, false);
