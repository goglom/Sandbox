#pragma once
#include "polygon.h"

class game_object
{
public:
	virtual ~game_object() = default;

	// Returns true if the object is dynamic
	[[nodiscard]] virtual bool is_dynamic() const = 0;

	// Returns hit box polygon for collision detection 
	[[nodiscard]] virtual const polygon& get_hit_box() const = 0;

	// Draw object
	virtual void draw() = 0;
};