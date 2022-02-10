#include "SDL_car_player_control.h"
#include <numbers>


SDL_car_player_control::SDL_car_player_control(
	const Uint8* keyboard_state_,
	const car_way& car_way,
	int throttle_up_key, 
	int throttle_down_key,
	int turn_right_key, 
	int turn_left_key, 
	int brake_key
)
		: car_control(car_way),
		  keyboard_state_(keyboard_state_),
		  throttle_up_key_(throttle_up_key),
		  throttle_down_key_(throttle_down_key),
		  turn_right_key_(turn_right_key),
		  turn_left_key_(turn_left_key),
		  brake_key_(brake_key)
{}

void SDL_car_player_control::get_control(dynamic_car_params& params)
{
	update_check_points(params);
	
	SDL_PumpEvents();
	const auto f_pi = std::numbers::pi_v<float>;

	if (keyboard_state_[throttle_up_key_])	// throttle up
	{
		if (params.throttle < 100.f)
		{
			params.throttle += 1.f + params.throttle / 100.f;
		}
	}
	if (keyboard_state_[throttle_down_key_]) // throttle down
	{
		if (params.throttle >= -30.f)
		{
			params.throttle -= 10.f;
		}
	}
	if (!keyboard_state_[throttle_up_key_] && !keyboard_state_[throttle_down_key_]) {
		if (params.throttle >= 1.f)
		{
			params.throttle *= 0.7f;
		}
		else if (params.throttle <= -1.f)
		{
			params.throttle += 1.f;
		}

	}
	if (keyboard_state_[brake_key_])	// brake
	{
		params.brake = 100.f;
		params.throttle = 0.f;
	}
	else
	{
		params.brake = 0.f;
	}

	// Steering 

	const float angle_add = f_pi / 70.f;
	const float angle_dec = f_pi / 100.f;
	const float steer_angle_lim = f_pi / 4.f;
	
	if (keyboard_state_[turn_left_key_])
	{
		if (params.steer_angle > -steer_angle_lim)
		{
			params.steer_angle -= angle_add;
		}
	}
	if (keyboard_state_[turn_right_key_])
	{
		if (params.steer_angle < steer_angle_lim)
		{
			params.steer_angle += angle_add;
		}
	}

	if (params.steer_angle > f_pi / 200.f)
	{
		params.steer_angle -= angle_dec;
	}
	else if (params.steer_angle < -f_pi / 200.f)
	{
		params.steer_angle += angle_dec;
	}
	else
	{
		params.steer_angle = 0.f;
	}
}
