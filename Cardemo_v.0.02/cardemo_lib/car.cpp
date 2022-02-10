#include "car.h"

#include <algorithm>
#include <cmath>

#include "vector_2d.h"

#include <iostream>


constexpr float low_velocity_resistance = 2500.f; //resistance factor for braking on low speed
constexpr bool draw_centers = true;	// if it's true drawing methods will draw points of shapes centers

enum wheels_names
{
	left_front = 0,
	right_front = 1,
	left_rear = 2,
	right_rear = 3,
	body = 4
};

				//**CLASS CAR_MODEL**//

car_model::car_model(const vector_2d<float>& position_wc, float angle, drawer& drawer,
	const color& body, const color& wheels, const car_type& car_type)
	: drawer_(drawer), body_color_(body), wheels_color_(wheels),
		cm_offset_(car_type.cm_offset, 0)
{
	wheel_offset_.emplace_back(car_type.wheel_offset_x, -car_type.wheel_offset_y);
	wheel_offset_.emplace_back(car_type.wheel_offset_x,  car_type.wheel_offset_y);
	wheel_offset_.emplace_back(-car_type.wheel_offset_x, -car_type.wheel_offset_y);
	wheel_offset_.emplace_back(-car_type.wheel_offset_x, car_type.wheel_offset_y);

	components_.emplace_back(wheel_offset_[left_front] + position_wc - cm_offset_, 
		car_type.wheel_length, car_type.wheel_width, angle);
	
	components_.emplace_back(wheel_offset_[right_front] + position_wc - cm_offset_, 
		car_type.wheel_length, car_type.wheel_width, angle);
	
	components_.emplace_back(wheel_offset_[left_rear] + position_wc - cm_offset_,
		car_type.wheel_length, car_type.wheel_width, angle);
	
	components_.emplace_back(wheel_offset_[right_rear] + position_wc - cm_offset_,
		car_type.wheel_length, car_type.wheel_width, angle);
	
	components_.emplace_back(position_wc, car_type.length, car_type.width, angle, 
		cm_offset_);
}

void car_model::move_model(const vector_2d<float>& position_wc, float angle, float steer_angle)
{
	
	components_[left_front].move_n_rotate(
		position_wc + wheel_offset_[left_front].get_rotated(angle) - cm_offset_.get_rotated(angle),
		angle + steer_angle);
	
	components_[right_front].move_n_rotate(
		position_wc + wheel_offset_[right_front].get_rotated(angle) - cm_offset_.get_rotated(angle),
		angle + steer_angle);

	components_[left_rear].move_n_rotate(
		position_wc + wheel_offset_[left_rear].get_rotated(angle) - cm_offset_.get_rotated(angle),
		angle);
	
	components_[right_rear].move_n_rotate(
		position_wc + wheel_offset_[right_rear].get_rotated(angle) - cm_offset_.get_rotated(angle),
		angle);

	components_[body].move_n_rotate(position_wc, angle);
}

void car_model::draw_model()
{
	drawer_.set_draw_color(wheels_color_);
	
	for (size_t i = 0; i < 4; ++i)
	{
		components_[i].draw(drawer_, draw_centers);
	}
	
	drawer_.set_draw_color(body_color_);
	components_[body].draw(drawer_, draw_centers);
}

const polygon& car_model::get_body() const
{
	return components_[body];
}

							//**CLASS CAR**//

float car::lateral_force_curve(float slip_angle, float tires_const, bool tires_slip) const
{
	auto lateral_force_factor = tires_const * slip_angle;

	lateral_force_factor = std::min(stat_params_.max_grip, lateral_force_factor);
	lateral_force_factor = std::max(-stat_params_.max_grip, lateral_force_factor);
	lateral_force_factor *= stat_params_.weight;

	if (tires_slip)
		lateral_force_factor /= 2.f;

	return lateral_force_factor;
}
template <typename T> float sgn(T val) {
	auto sign = (T(0) < val) - (val < T(0));
	return sign == 0 ? 1.f : static_cast<float>(sign);
}

static vector_2d<float> low_vel_resist_function(const vector_2d<float>& velocity)
{
	const float limit = 0.1f;
	
	return {
		abs(velocity.x) < limit
			? 0
			: low_velocity_resistance / velocity.x,
		abs(velocity.y) < limit
			? 0
			: low_velocity_resistance / velocity.y
	};
}
								
car::car(
	const vector_2d<float>& position_wc,
	float angle,
	const color& body_color,
	const color& wheels_color,
	car_control& control,
	drawer& drawer,
	const car_type& car_type
)
	:
	dynamic_object(position_wc, angle),
	controller_(control),
	stat_params_(car_type),
	dyn_params_( position_wc_, velocity_wc_ ),
	model_(position_wc, angle, drawer, body_color, wheels_color, stat_params_)
	
{}

float car::get_mass() const
{
	return stat_params_.weight * 2.f / gravitational_acceleration;
}

void car::process_phys()
{
	// Get car dynamic parameters by external input
	controller_.get_control(dyn_params_);
	
	const auto velocity(velocity_wc_.get_rotated(-angle_));
	vector_2d<float> low_vel_res(0, 0);

	float slip_angle_front;
	float slip_angle_rear;
	const auto cm_to_front = stat_params_.length / 2.f - stat_params_.cm_offset;
	const auto cm_to_rear = stat_params_.length / 2.f + stat_params_.cm_offset;

	if (abs(velocity.x) < 0.5f)
	{
		slip_angle_front = slip_angle_rear = 0;
	}
	else
	{
		slip_angle_front = atan2f(
			velocity.y + angular_velocity_ * cm_to_front,
			abs(velocity.x)) - dyn_params_.steer_angle * sgn(velocity.x);


		slip_angle_rear = atan2f(
			velocity.y - angular_velocity_ * cm_to_rear, 
			abs(velocity.x)
		);
	}

	// lateral force on wheels 
	const auto front_lateral_force = 
		lateral_force_curve(slip_angle_front, stat_params_.slip_factor_front, stat_params_.front_slip);
	
	const auto rear_lateral_force =
		lateral_force_curve(slip_angle_rear, stat_params_.slip_factor_rear, stat_params_.rear_slip);

	// longitudinal force on rear wheels - very simple traction model
	auto traction_force = stat_params_.engine_power_factor * (dyn_params_.throttle - dyn_params_.brake * sgn(velocity.x));

	if (abs(traction_force) < 0.5)
		traction_force = 0.f;

	if (stat_params_.rear_slip)
	{
		traction_force *= 0.5f;
	}

	//Extra braking power for low speeds.
	if (abs(dyn_params_.throttle) < 0.5f)
	{
		low_vel_res = low_vel_resist_function(velocity);
	}

	const vector_2d<float> resistance = (
		velocity * stat_params_.rolling_resistance
		+ low_vel_res
		+ velocity * velocity.length() * stat_params_.air_resistance) * -1.f;

	// Sum forces
	const vector_2d<float> force = {
		traction_force + resistance.x,
		cos(dyn_params_.steer_angle) * front_lateral_force + rear_lateral_force + resistance.y - dyn_params_.brake * sgn(velocity.y)
	};

	// Torque on body from lateral forces
	const auto torque = cm_to_front * front_lateral_force - cm_to_rear * rear_lateral_force;

	// Acceleration
	const auto acceleration = force * 2.f * gravitational_acceleration / stat_params_.weight;

	angular_acceleration_ = torque / stat_params_.inertia;

	// Velocity and position
	acceleration_wc_ = acceleration.get_rotated(angle_);
}

const polygon& car::get_hit_box() const
{
	return  model_.get_body();
}

void car::draw()
{
	model_.move_model(get_position(), angle(), dyn_params_.steer_angle);
	model_.draw_model();
}