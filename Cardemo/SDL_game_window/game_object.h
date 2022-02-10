#pragma once
#include "polygon.h"

class game_object
{
public:
	virtual ~game_object() = default;

	// TODO: May be it unused method 
	[[nodiscard]] bool is_dynamic() const;

	virtual const polygon& get_hit_box() const = 0;
	virtual void draw() = 0;
};

