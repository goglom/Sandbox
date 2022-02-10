#pragma once
#include "game_object.h"

#define $$ [[nodiscard]]

class static_object :
	public game_object
{
public:
	
	$$ bool is_dynamic() const override { return false; }

	// Move static object to given position at the scene
	virtual void move_to(const vector_2d<float>& position_wc) = 0;

	// Rotate static object to given position at the scene
	virtual void rotate(float angle) = 0;

	// Returns collision elasticity factor for collisions treatment
	$$ virtual float get_collision_elasticity_factor() const = 0;

	// Returns collision friction factor for collisions treatment
	$$ virtual float get_collision_friction_factor() const = 0;

};

#undef $$