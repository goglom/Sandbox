#pragma once

struct dynamic_car_params
{
	float steer_angle_;
	float throttle_;
	float brake_;
};


class car_control
{
public:
	virtual void get_control(dynamic_car_params& params) = 0;
};

