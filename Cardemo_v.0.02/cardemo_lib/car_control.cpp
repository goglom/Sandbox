#include "car_control.h"

void car_control::update_check_points(const dynamic_car_params& params)
{
	auto [c_p, radius] = car_way_.get_check_point(cur_cp_);
	
	if ((params.position_ - c_p).length() < radius)
	{
		++cur_cp_;
		cur_cp_ %= car_way_.get_cp_count();

		if (cur_cp_ == 1)
		{
			++cur_round_;
		}
	}
}

vector_2d<float> car_control::get_cur_cp() const
{
	return car_way_.get_check_point(cur_cp_).first;
}

car_control::car_control(const car_way& car_way)
	: car_way_(car_way), cur_cp_(0), cur_round_(0)
{}

size_t car_control::get_rounds() const
{
	return cur_round_;
}

size_t car_control::get_cp_count() const
{
	return cur_cp_;
}
