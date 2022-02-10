#pragma once
#include "car_control.h"
#include "car_way.h"

class bot_car_control final :
	public car_control
{

public:

	explicit bot_car_control(const car_way& car_way);
	void get_control(dynamic_car_params& params) override;
	
};

