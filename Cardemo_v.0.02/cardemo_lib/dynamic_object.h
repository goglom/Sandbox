#pragma once
#include "game_object.h"
#include "static_object.h"
#include "vector_2d.h"

#define $$ [[nodiscard]]

class dynamic_object :
	public game_object
{
protected:
	vector_2d<float> position_wc_;
	vector_2d<float> velocity_wc_{};
	vector_2d<float> acceleration_wc_{};

	float angle_;
	float angular_velocity_ = 0;
	float angular_acceleration_ = 0;

	inline static float time_step_ = 0.01f;
	
public:

	explicit dynamic_object(const vector_2d<float>& pos, float angle);

	// Calculate by integrating of static parameters of the object
	virtual void process_move();

	// Calculate dynamic parameters of the object
	virtual void process_phys() = 0;

	// Function makes collision treatment between dynamic and static objects
	friend void treat_collision(dynamic_object& dyn_obj, const static_object& stat_obj);

	// Returns mass of the object in kg 
	$$ virtual float get_mass() const = 0;

	$$ bool is_dynamic() const override { return true; }

	// Returns 2d radius-vector of object position in World Coordinates (wc)
	$$ vector_2d<float> get_position() const { return position_wc_; }

	// Returns 2d vector of object velocity in World Coordinates (wc)
	$$ vector_2d<float> get_velocity() const { return velocity_wc_; }

	// Returns rotate angle relative the x-axis
	$$ float angle() const { return  angle_; }

	// Returns value of object angular velocity
	$$ float& get_angular_velocity() { return angular_velocity_; }
};

#undef $$