#include "dynamic_object.h"
#include <numbers>

dynamic_object::dynamic_object(const vector_2d<float>& pos, float angle) :
	position_wc_(pos), angle_(angle)
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

void treat_collision(dynamic_object& dyn_obj, const static_object& stat_obj)
{
	const auto nearest_vertex = dyn_obj.get_position();
	
	const auto barrier_normal =
		stat_obj.get_hit_box().get_nearest_normal(nearest_vertex).get_normalized();

	
	const auto barrier_long = barrier_normal.get_orthogonal();

	const auto longitudinal_velocity_component = (barrier_normal * dyn_obj.velocity_wc_) * barrier_normal;
	const auto transverse_velocity_component = (barrier_long * dyn_obj.velocity_wc_) * barrier_long;

	dyn_obj.velocity_wc_ -= longitudinal_velocity_component * (1.f + stat_obj.get_collision_elasticity_factor());
	dyn_obj.velocity_wc_ -= transverse_velocity_component * (1.f - stat_obj.get_collision_friction_factor());

	dyn_obj.position_wc_ += barrier_normal * 0.05f;
}