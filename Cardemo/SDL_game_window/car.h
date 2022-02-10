#pragma once
#include "dynamic_object.h"
#include "car_control.h"
#include "polygon.h"
#include "vector_2d.h"
#include "SDL_drawer.h"

constexpr float gravitational_acceleration = 9.81f;

struct car_type
{
	const float cm_to_front;				// in m, distance from center of mass (CM) to front axle
	const float cm_to_rear;					// in m, idem to rear axle
	const float weight;						// in N (Newtons), weight for front or rear wheel pair
	const float inertia;					// in kg.m, moment of inertia of the car
	const float length, width;				// in m, length and width of car body
	const float wheel_length, wheel_width;	// in m
	const float	air_resistance;				// factor for air resistance (drag)
	const float rolling_resistance;			// factor for rolling resistance
	const float engine_power_factor;
	const float slip_factor_rear;		//cornering stiffness factor
	const float slip_factor_front;	//cornering stiffness factor
	const float max_grip;				//maximum (normalized) friction force, =diameter of friction circle
	const bool rear_slip;
	const bool front_slip;
	const float wheel_pos_x;
	const float wheel_pos_y;
};

namespace car_types
{
	constexpr car_type simple_car = {
		2.f,
		1.f,
		1500.f * gravitational_acceleration / 2.f,
		1500.f,
		3.f,
		1.5f,
		0.7f,
		0.3f,
		5.0f,
		30.f,
		150.f,
		-5.2f,
		-5.f,
		3.f,
		false,
		false,
		2.8f,
		0.75f
	};

}

class car :
	public dynamic_object
{
	car_type static_params_;
	car_control& control_;
	dynamic_car_params dynamic_params_{ 0.f, 0.f, 0.f };

	SDL_drawer& drawer_;
	rectangle car_body_;

	SDL_Color body_col_;
	SDL_Color wheels_col_;


	std::vector<rectangle> wheels_;
	std::vector<vector_2d<float>> wheel_pos_;
	
	float lateral_force_curve(float slip_angle, float tires_const, bool tires_slip) const;
	void set_wheels_pos();

public:
	car(
		const vector_2d<float>& position,
		const SDL_Color& body_color,
		const SDL_Color& wheels_color,
		const car_control& control,
		SDL_drawer& drawer,
		const car_type& car_type = car_types::simple_car
	);

	void process_phys() override;
	const polygon& get_hit_box() const override;
	void draw() override;
};