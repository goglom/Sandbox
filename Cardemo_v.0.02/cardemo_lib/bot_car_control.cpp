#include "bot_car_control.h"
#include <cmath>
#include <numbers>

bot_car_control::bot_car_control(const car_way& car_way)
	: car_control(car_way)
{}

void bot_car_control::get_control(dynamic_car_params& params)
{
	update_check_points(params);

	const auto to_check_point = (get_cur_cp() - params.position_).get_normalized();

	const auto velocity_tr = params.velocity_ - (params.velocity_ * to_check_point) * to_check_point;

	const auto steer_factor = (velocity_tr * to_check_point.get_orthogonal()) / params.velocity_.length();

	const auto f_pi = std::numbers::pi_v<float>;
	
	if (abs(params.steer_angle) <= f_pi / 6)
	{
		if (params.velocity_.length() != 0.f)
		{
			params.steer_angle = sin(steer_factor * f_pi / 2.f) * f_pi / 10.f;
		}

		if (params.velocity_.length()!= 0.f)
		{
			params.throttle = 70 * cos(steer_factor * f_pi / 2.f);
		}
		else
		{
			params.throttle = 1.f;
		}
	}
}
