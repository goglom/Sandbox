#include "car.h"

#include <algorithm>
#include <cmath>

#include "vector_2d.h"

constexpr float low_velocity_resistance = 1200.f;

//TODO : REFACTOR ALL DEFINES

//#define DRAG				5.0f		//factor for air resistance (drag)
//#define RESISTANCE			30.f		//factor for rolling resistance
//#define LOW_VEL_RESISTANCE		1200.f	//coefficient for smooth braking
//#define CA_R				(-5.2f)		//cornering stiffness
//#define CA_F				(-5.f)		//cornering stiffness
//#define MAX_GRIP			3.f			//maximum (normalized) friction force, =diameter of friction circle
//#define GRAVITATIONAL_ACCELERATION 9.8f //You know what that means
//#define SOME_SHIT			150.f		//Just magic number

	//resistance factor for braking on low speed


				//**STATIC FUNCTIONS AND PRIVATE METHODS**//

float car::lateral_force_curve(float slip_angle, float tires_const, bool tires_slip) const
{
	auto lateral_force_factor = tires_const * slip_angle;

	lateral_force_factor = std::min(static_params_.max_grip, lateral_force_factor);
	lateral_force_factor = std::max(-static_params_.max_grip, lateral_force_factor);
	lateral_force_factor *= static_params_.weight;

	if (tires_slip)
		lateral_force_factor /= 2.f;

	return lateral_force_factor;
}

void car::set_wheels_pos()
{
	vector_2d<float> temp_wheel_pos(static_params_.wheel_pos_x, static_params_.wheel_pos_y);
	wheel_pos_[0] = position_wc_ + temp_wheel_pos;

	temp_wheel_pos.y *= -1;
	wheel_pos_[1] = position_wc_ + temp_wheel_pos;

	temp_wheel_pos.x *= -1;
	wheel_pos_[2] = position_wc_ + temp_wheel_pos;

	temp_wheel_pos.y *= -1.f;
	wheel_pos_[3] = position_wc_ + temp_wheel_pos;

	for (size_t i = 0; i < wheel_pos_.size(); ++i)
	{
		wheels_[i].move_to(wheel_pos_[i]);
		
		if (i < 2)
		{
			wheels_[i].rotate(angle_ + dynamic_params_.steer_angle_);
		}
		else
		{
			wheels_[i].rotate(angle_);
		}
	}
}

car::car(const vector_2d<float>& position, const SDL_Color& body_color, const SDL_Color& wheels_color,
         const car_control& control, SDL_drawer& drawer, const car_type& car_type)
:
	dynamic_object(position),
	static_params_(car_type),
	control_(control_),
	drawer_(drawer),
	car_body_(
		position,
		car_type.length,
		car_type.width,
		0,
		{ car_type.length / 2.f - car_type.cm_to_front, 0 }
	),
	body_col_(body_color),
	wheels_col_(wheels_color)
{
	wheel_pos_.resize(4);
	wheels_.resize(4);
	
	set_wheels_pos();
}


template <typename T> int sgn(T val) {
	auto sign = (T(0) < val) - (val < T(0));
	return sign == 0 ? 1.f : sign;
}

static vector_2d<float> low_vel_resist_function(const vector_2d<float>& velocity)
{
	return {
		abs(velocity.x) < 0.05
			? 0
			: low_velocity_resistance / velocity.x,
		abs(velocity.y) < 0.05
			? 0
			: low_velocity_resistance / velocity.y
	};
}


				//**PUBLIC METHODS**//

void car::process_phys()
{
	// Get car dynamic parameters by external input
	control_.get_control(dynamic_params_);
	
	const auto velocity(velocity_wc_.get_rotated(-angle_));
	vector_2d<float>	low_vel_res;

	float	slip_angle_front;
	float	slip_angle_rear;

	if (fabsf(velocity.x) < 0.1f)
	{
		slip_angle_front = slip_angle_rear = 0;
	}
	else
	{
		slip_angle_front = atan2f(
			velocity.y + angular_velocity_ * static_params_.cm_to_front,
			abs(velocity.x) - dynamic_params_.steer_angle_ * sgn(velocity.x)
		);

		slip_angle_rear = atan2f(
			velocity.y - angular_velocity_ * static_params_.cm_to_rear, 
			abs(velocity.x)
		);
	}

	// lateral force on wheels 
	const auto front_lateral_force = 
		lateral_force_curve(slip_angle_front, static_params_.slip_factor_front, static_params_.front_slip);
	
	const auto rear_lateral_force =
		lateral_force_curve(slip_angle_rear, static_params_.slip_factor_rear, static_params_.rear_slip);

	// longitudinal force on rear wheels - very simple traction model
	auto traction_force = static_params_.engine_power_factor * (dynamic_params_.throttle_ - dynamic_params_.brake_ * sgn(velocity.x));

	if (static_params_.rear_slip)
	{
		traction_force *= 0.5f;
	}

	//TODO: perhaps this module needs to be improved
	//Extra braking power for low speeds.
	if (fabsf(dynamic_params_.throttle_) < 0.5f)
	{
		low_vel_res = low_vel_resist_function(velocity);
	}											  

	const auto resistance
		= velocity * -static_params_.rolling_resistance
		+ velocity * velocity.length() * static_params_.air_resistance
		+ low_vel_res;

	// sum forces
	const vector_2d<float> force = {
		traction_force + resistance.x,
		cosf(dynamic_params_.steer_angle_) * front_lateral_force + rear_lateral_force + resistance.y - dynamic_params_.brake_ * sgn(velocity.y)
	};

	// torque on body from lateral forces
	const auto torque = static_params_.cm_to_front * front_lateral_force - static_params_.cm_to_rear * rear_lateral_force;

	// Acceleration
	const auto acceleration = force * 2.f * gravitational_acceleration / static_params_.weight;
	angular_acceleration_ = torque / static_params_.inertia;

	// Velocity and position
	acceleration_wc_ = acceleration.get_rotated(angle_);
}

const polygon& car::get_hit_box() const
{
	return car_body_;
}

void car::draw()
{
	drawer_.set_draw_color(body_col_);

	car_body_.move_to(position_wc_);
	car_body_.rotate(angle_);
	car_body_.draw(drawer_);

	drawer_.set_draw_color(wheels_col_);
	set_wheels_pos();

	for (auto& wheel : wheels_)
	{
		wheel.draw(drawer_);
	}	
}