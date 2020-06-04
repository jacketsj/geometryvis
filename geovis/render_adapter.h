#pragma once

#include <string>

#include "col.h"
#include "point.h"
#include "properties.h"

namespace render_adapter {
extern void set_color(const col::col& c);
extern void draw_line(const pt<double>& p1, const pt<double>& p2,
											const properties& prop);
extern void draw_circle(const pt<double>& p, const double& r,
												const properties& prop);
extern void draw_point(const pt<double>& p, const properties& prop);
extern void draw_rect(const pt<double>& centre, const pt<double>& size,
											const properties& prop);
extern pt<double> get_text_dimensions(const std::string& s);
extern int font;
extern void init_font();
extern void draw_text(const pt<double>& p, const std::string& s,
											const properties& prop);
double get_time();
} // namespace render_adapter
