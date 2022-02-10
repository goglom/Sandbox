#pragma once
#include "static_object.h"

#define $$ [[nodiscard]]

class trace_barrier final :
	public static_object
{
	polygon box_;
	drawer& drawer_;
	color draw_color_;
	float col_el_factor_;
	float col_fr_factor_;
	
public:

	// Construct trace barrier with next parameters:
	// 'box' - polygon of barrier hit box
	// 'drawer' - drawer class drawing this object 'box'
	// 'color' - color of box bound
	// 'collision_elasticity_factor' - ratio of normal projection of velocities after collision and before
	// 'collision_friction_factor' - ratio of transverse projection of velocities after collision and before
	trace_barrier(
		const polygon& box, 
		drawer& drawer, 
		const color& color,
		float collision_elasticity_factor,
		float collision_friction_factor
	);

	$$ const polygon& get_hit_box() const override;
	void draw() override;
	void move_to(const vector_2d<float>& position_wc) override;
	void rotate(float angle) override;
	$$ float get_collision_elasticity_factor() const override;
	$$ float get_collision_friction_factor() const override;
	
};

#undef $$