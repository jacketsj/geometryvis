#include "point.h"

#include "geometry.h"
#include "properties.h"
#include "render_adapter.h"

template <> void pt<double>::draw() const {
	render_adapter::draw_point(*this, this->prop);
}
