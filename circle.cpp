#include "circle.h"

#include "geometry.h"
#include "line.h"
#include "point.h"
#include "properties.h"
#include "render_adapter.h"

template <> void circle<double>::draw() const {
	render_adapter::draw_circle(this->p, sqrt(double(r2)), this->prop);
}
