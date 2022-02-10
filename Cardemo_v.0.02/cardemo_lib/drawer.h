#pragma once
#include "vector_2d.h"

struct color
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
};


class drawer
{
public:

	// Draw line connecting two point by their radius-vectors
	virtual  void draw_line(const vector_2d<float>& point_1, 
		const vector_2d<float>& point_2) const = 0;

	// Draw point by his radius-vector
	virtual void draw_point(const vector_2d<float>& point) const = 0;

	// Set color of drawing
	virtual void set_draw_color(const color&) = 0;
	
	virtual ~drawer() = default;
};