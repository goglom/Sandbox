#pragma once
#include "vector_2d.h"
#include "drawer.h"


class shape
{
protected:
	float radius_;
	vector_2d<float> center_;
	
public:
	virtual ~shape() = default;
	explicit shape(const vector_2d<float>& center) : radius_(0), center_(center)
	{}
	
	virtual void draw(const drawer& drawer, bool draw_center) = 0;
	virtual void rotate(float angle) = 0;
	virtual void move_to(const vector_2d<float>& position) = 0;

	float get_radius() const
	{
		return radius_;
	}

	const vector_2d<float>& get_center() const
	{
		return center_;
	}
};


// Returns false if the shapes do not intersect
inline bool approx_collision_detect(const shape& first, const shape& second)
{
	return (first.get_center() - second.get_center()).length() <= first.get_radius() + second.get_radius();
}

// Overload this function for derived classes
inline bool accurate_convex_collision_detect()
{
	return true;
}
