#include "game_scene.h"
#include "dynamic_object.h"

game_scene::game_scene(std::initializer_list<static_object*> static_list,
	std::initializer_list<dynamic_object*> dynamic_list, car_way& car_way)
	:
	static_obj_(static_list),
	dynamic_obj_(dynamic_list),
	car_way_(car_way),
	cp_draw_flag_(false)
{}

void game_scene::draw_scene()
{
	for (auto* dynamic_obj : dynamic_obj_)
	{
		(*dynamic_obj).draw();
	}

	for (auto static_obj : static_obj_)
	{
		(*static_obj).draw();
	}

	if (cp_draw_flag_)
	{
		car_way_.draw_cp();
	}
}

class A
{
	int* array_;

public:

	A(size_t size)
	{
		array_ = new int[size];
	}


	
};


void game_scene::collision_process()
{
	for (auto* dynamic_obj : dynamic_obj_)
	{
		for (auto* static_obj : static_obj_)
		{
			if (approx_collision_detect(
				(*dynamic_obj).get_hit_box(),
				(*static_obj).get_hit_box())
				)
			{
				if (accurate_convex_collision_detect(
					(*dynamic_obj).get_hit_box(),
					(*static_obj).get_hit_box())
					)
				{
					treat_collision(*dynamic_obj, *static_obj);
				}
			}

		}
	}
}

void game_scene::move_process()
{
	for (auto* dynamic_obj : dynamic_obj_)
	{
		(*dynamic_obj).process_move();
	}
}

void game_scene::turn_drawing_check_points()
{
	if (cp_draw_flag_)
	{
		cp_draw_flag_ = false;
	}
	else
	{
		cp_draw_flag_ = true;
	}

}

game_scene::~game_scene()
{
	for (auto* dynamic_obj : dynamic_obj_)
	{
		delete dynamic_obj;
	}

	for (auto* static_obj : static_obj_)
	{
		delete static_obj;
	}
}