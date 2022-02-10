#pragma once
#include "car_control.h"
class car_player_control :
	public car_control
{
	void get_control(dynamic_car_params& params) override
	{
		return;
	}
};

