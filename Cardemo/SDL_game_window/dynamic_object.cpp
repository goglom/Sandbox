#include "dynamic_object.h"

#include <numbers>

dynamic_object::dynamic_object(const vector_2d<float>& pos) :
	position_wc_(pos)
{}

void dynamic_object::process_move()
{
	process_phys();
	
	velocity_wc_ += acceleration_wc_ * time_step_;
	position_wc_ += velocity_wc_ * time_step_;

	angular_velocity_ += angular_acceleration_ * time_step_;
	angle_ += angular_velocity_ * time_step_;

	const auto m_2_pi = 2.f * std::numbers::pi_v<float>;
	
	if (angle_ > m_2_pi)
	{
		angle_ -= m_2_pi;
	}
	else if (angle_ < -m_2_pi)
	{
		angle_ += m_2_pi;
	}
}
