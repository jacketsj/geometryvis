#include "rect.h"

#include "geometry.h"
#include "properties.h"
#include "render_adapter.h"

template <> void rect<double>::draw() const {
	render_adapter::draw_rect(p, sz, this->prop);
}
