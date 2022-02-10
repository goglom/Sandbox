#pragma once

#include "dynamic_object.h"
#include "static_object.h"
#include "car_way.h"

#include <list>

class game_scene
{
	std::list<static_object*> static_obj_;
	std::list<dynamic_object*> dynamic_obj_;
	car_way& car_way_;
	bool cp_draw_flag_;

public:

	game_scene(
		std::initializer_list<static_object*> static_list,
		std::initializer_list<dynamic_object*>  dynamic_list,
		car_way& car_way
	);

	// Draw all object at the scene
	void draw_scene();

	// Detection and resolving collisions
	void collision_process();

	// Move process for all dynamic objects at the scene
	void move_process();

	// Turns drawing dots at check points places
	void turn_drawing_check_points();

	~game_scene();
};

