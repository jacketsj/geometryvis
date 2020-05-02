#include "render_adapter.h"

#include <sl.h>

#include "col.h"
#include "point.h"
#include "properties.h"

void render_adapter::set_color(const col::col& c) {
	slSetForeColor(c.r, c.g, c.b, c.a);
}
void render_adapter::draw_line(const pt<double>& p1, const pt<double>& p2,
															 const properties& prop) {
	set_color(prop.c);
	slLine(p1.x, p1.y, p2.x, p2.y);
}
void render_adapter::draw_circle(const pt<double>& p, const double& r,
																 const properties& prop) {
	set_color(prop.c);
	slCircleOutline(p.x, p.y, r, int(r * r));
}
void render_adapter::draw_point(const pt<double>& p, const properties& prop) {
	set_color(prop.c);
	// slPoint(p.x, p.y);
	slCircleFill(p.x, p.y, 4, 8);
}
void render_adapter::draw_rect(const pt<double>& centre, const pt<double>& size,
															 const properties& prop) {
	set_color(prop.c);
	slRectangleFill(centre.x, centre.y, size.x, size.y);
}
pt<double> render_adapter::get_text_dimensions(const std::string& s) {
	double width = slGetTextWidth(s.c_str());
	double height = slGetTextHeight(s.c_str());
	return pt<double>(width, height);
}
int render_adapter::font;
void render_adapter::init_font() {
	font = slLoadFont("ttf/white_rabbit.ttf");
	slSetFont(font, 12);
	slSetTextAlign(SL_ALIGN_LEFT);
}
void render_adapter::draw_text(const pt<double>& p, const std::string& s,
															 const properties& prop) {
	set_color(prop.c);
	slSetFontSize(prop.font_size);
	slText(p.x, p.y, s.c_str());
}
double render_adapter::get_time() { return slGetTime(); }
