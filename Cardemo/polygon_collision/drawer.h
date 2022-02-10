#pragma once
#include "vector_2d.h"

class drawer
{
public:
	virtual  void draw_line(const vector_2d<float>& point_1, 
		const vector_2d<float>& point_2) const = 0;

	virtual void draw_point(const vector_2d<float>& point) const = 0;
	
	//virtual ~drawer() = 0;
};
