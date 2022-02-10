#pragma once
#include "dynamic_object.h"
#include "polygon.h"
#include "vector_2d.h"
#include "SDL_drawer.h"
#include "car_control.h"

#include <vector>

#define $$ [[nodiscard]]

constexpr float gravitational_acceleration = 9.81f;

struct car_type final
{
	const float cm_offset;					// in m, the offset between Center of Mass (cm) and geometry center
	const float weight;						// in N (Newtons), weight for front or rear wheel pair
	const float inertia;					// in kg.m, moment of inertia of the car
	const float length, width;				// in m, length and width of car body
	const float wheel_length, wheel_width;	// in m
	const float	air_resistance;				// factor for air resistance (drag)
	const float rolling_resistance;			// factor for rolling resistance
	const float engine_power_factor;		// factor of engine max power
	const float slip_factor_rear;			// cornering stiffness factor
	const float slip_factor_front;			// cornering stiffness factor
	const float max_grip;					// maximum (normalized) friction force, =diameter of friction circle
	const bool rear_slip;					// turn slip of rear wheels
	const bool front_slip;					// turn slip of front wheels
	const float wheel_offset_x;				// in m, the x component of offset between CG and wheel by absolute value 
	const float wheel_offset_y;				// in m, the y component of offset between CG and wheel by absolute value
};

namespace car_types
{
	constexpr car_type simple_car = {
		-0.2f,
		1500.f * gravitational_acceleration / 2.f,
		1500.f,
		3.f,
		1.5f,
		0.7f,
		0.3f,
		5.0f,
		30.f,
		150.f,
		-6.2f,
		-6.f,
		3.f,
		false,
		false,
		1.f,
		0.75f
	};

}

class car_model final
{
	std::vector<rectangle> components_;
	std::vector<vector_2d<float>> wheel_offset_;
	drawer& drawer_;
	color body_color_;
	color wheels_color_;
	vector_2d<float> cm_offset_;
	
public:

	car_model(const vector_2d<float>& position_wc, float angle, drawer& drawer, 
		const color& body, const color& wheels, const car_type& car_type);

	void move_model(const vector_2d<float>& position_wc, float angle, float steer_angle);

	void draw_model();

	$$ const polygon& get_body() const;
	
};

class car final :
	public dynamic_object
{
	car_control& controller_;
	car_type stat_params_;
	dynamic_car_params dyn_params_;
	car_model model_;
	
	$$ float lateral_force_curve(float slip_angle, float tires_const, bool tires_slip) const;
	
public:
	car(
		const vector_2d<float>& position_wc,
		float angle,
		const color& body_color,
		const color& wheels_color,
		car_control& control,
		drawer& drawer,
		const car_type& car_type = car_types::simple_car
	);

	$$ float get_mass() const override;
	
	void process_phys() override;
	
	$$ const polygon& get_hit_box() const override;
	
	void draw() override;
};

#undef $$