#pragma once

#include "vector_2d.h"

struct dynamic_car_params final
{
	float steer_angle;
	float throttle;
	float brake;
	const vector_2d<float>& position_;
	const vector_2d<float>& velocity_;

	dynamic_car_params(const vector_2d<float>& position, const vector_2d<float>& velocity)
		:
		steer_angle(0.f),
		throttle(0.f),
		brake(0.f),
		position_(position),
		velocity_(velocity)
	{}
};

