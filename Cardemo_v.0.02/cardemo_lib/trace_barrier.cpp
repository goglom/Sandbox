#include "trace_barrier.h"

trace_barrier::trace_barrier(
	const polygon& box,
	drawer& drawer,
	const color& color,
	float collision_elasticity_factor,
	float collision_friction_factor
)
	:	box_(box),
		drawer_(drawer),
		draw_color_(color),
		col_el_factor_(collision_elasticity_factor),
		col_fr_factor_(collision_friction_factor)
{}

const polygon& trace_barrier::get_hit_box() const
{
	return box_;
}

void trace_barrier::draw()
{
	drawer_.set_draw_color(draw_color_);
	box_.draw(drawer_);
}

void trace_barrier::move_to(const vector_2d<float>& position_wc)
{
	box_.move_to(position_wc);
}

void trace_barrier::rotate(float angle)
{
	box_.rotate(angle);
}

float trace_barrier::get_collision_elasticity_factor() const
{
	return col_el_factor_;
}

float trace_barrier::get_collision_friction_factor() const
{
	return col_fr_factor_;
}
