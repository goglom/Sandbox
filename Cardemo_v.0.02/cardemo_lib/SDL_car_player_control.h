#pragma once
#include <SDL.h>
#include "car_control.h"

class SDL_car_player_control final :
	public car_control
{
	const Uint8* keyboard_state_;
	int throttle_up_key_;
	int throttle_down_key_;
	int turn_right_key_;
	int turn_left_key_;
	int brake_key_;
	
public:
	
	SDL_car_player_control(
		const Uint8* keyboard_state,
		const car_way& car_way,
		int throttle_up_key,
		int throttle_down_key,
		int turn_right_key,
		int turn_left_key,
		int brake_key
	);
	
	void get_control(dynamic_car_params& params) override;
};

