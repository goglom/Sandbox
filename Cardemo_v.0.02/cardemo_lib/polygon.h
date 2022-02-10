#pragma once
#include "shape.h"
#include "vector_2d.h"
#include <vector>

#define $$ [[nodiscard]]

class polygon :
	public shape
{
	// Contains the coordinates of the vertices relative to the center of rotation
	const std::vector<vector_2d<float>> rel_vertices_;

	// Contains vertex coordinates in WC
	std::vector<vector_2d<float>> wc_vertices_;

	// The bool flag activated when calling the constructor.
	// If it's true 
	bool is_convex_;

	// Returns true if the polygon are convex
	$$ bool bulge_check() const;

public:
	polygon() = default;
	
	// Constructs a polygon at the vertices of coordinates relative to its center
	// 'center' - link for the position of the center of rotation in world coordinates (WC)
	// 'angle'	- angle of rotation relative to the center
	// 'initializer_list' - contains a list of coordinates of the vertices of the polygon relative to its center
	polygon(const vector_2d<float>& center, std::initializer_list<vector_2d<float>> list, float angle = 0);

	// Returns count of polygon's vertex
	$$ size_t vertex_count() const;

	// Drawing polygon by drawer interface
	void draw(const drawer& drawer, bool draw_center = false) override;

	// Rotating polygon by angle relative the center
	void rotate(float angle) override;

	// Moving the center of polygon and his vertices
	void move_to(const vector_2d<float>& position) override;

	// Moving to position and rotating by angle the polygon .
	void move_n_rotate(const vector_2d<float>& position, float angle) override;
	
	// Returns true if the polygon is convex 
	$$ bool is_convex() const;
	
	// Returns polygon's edge by index
	$$ vector_2d<float> get_edge(size_t index) const;

	// Returns list of edges 2d vectors
	$$ std::vector<vector_2d<float>> build_edges() const;

	// Returns pair coordinates at axis of this polygon projection.
	$$ std::pair<float, float> orthographic_projection(const vector_2d<float>& axis_vector) const;

	// Returns normal of nearest edge to given point
	$$ vector_2d<float> get_nearest_normal(const vector_2d<float>& point) const;

	$$ vector_2d<float> find_nearest_point(const vector_2d<float>& point) const;

	$$ vector_2d<float> get_wc_vertex(size_t index) const;
};

// Returns true if this and second polygons have already collided.
// Using separate axis theorem.
bool accurate_convex_collision_detect(const polygon& first, const polygon& second);

class rectangle final :
	public polygon
{
public:

	// Construct rectangle with next parameters:
	// 'rot_center' - coordinate of center of rotation.
	// 'length' & 'width' - length for x-axis and width for y - axis.
	// 'angle' - start angle of rotation relative center of rotation.
	// 'offset' - offset above the geometry center of rectangle and center of rotation.
	// Vertex order:
	// 2-------1
	// |	   |
	// |	   |
	// 3-------4
	//
	rectangle() = default;
	
	rectangle(const vector_2d<float>& rot_center, 
		float length, 
		float width,
		float angle = 0,
		const vector_2d<float>& offset = vector_2d<float>(0.f ,0.f)
		)
	: polygon (
			rot_center,
		{
		          { length / 2 - offset.x,  width / 2 - offset.y},
		          {-length / 2 - offset.x,  width / 2 - offset.y},
		          {-length / 2 - offset.x, -width / 2 - offset.y},
		          { length / 2 - offset.x, -width / 2 - offset.y}
	          },
			angle
		)
	{}
};

#undef $$