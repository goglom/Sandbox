#pragma once
#include "game_object.h"
#include "vector_2d.h"

struct surface
{
	
};

class dynamic_object :
	public game_object
{
protected:
	vector_2d<float> position_wc_;
	vector_2d<float> velocity_wc_{};
	vector_2d<float> acceleration_wc_{};

	float angle_ = 0;
	float angular_velocity_ = 0;
	float angular_acceleration_ = 0;
	
	static float time_step_;
	
public:

	explicit dynamic_object(const vector_2d<float>& pos);
	virtual void process_move();
	virtual void process_phys() = 0;

	[[nodiscard]] vector_2d<float> position() const { return position_wc_; }
	[[nodiscard]] vector_2d<float> velocity() const { return velocity_wc_; }

	[[nodiscard]] float angle() const { return  angle_; }
	[[nodiscard]] float angular_velocity() const { return angular_velocity_; }	
};

