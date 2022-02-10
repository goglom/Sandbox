#include "polygon.h"
#include <functional>
#include <algorithm>

using namespace std;

template <typename T>
static int sgn(T val)
{
	auto a = (T(0) < val) - (val < T(0));
	return a == 0 ? 1 : a;
}

// Returns maximal radius of the polygon
static float find_radius(const vector<vector_2d<float>>& vertices, const vector_2d<float> center)
{
	float max_radius = 0;

	for (const auto& vertex : vertices)
	{
		max_radius = max(max_radius, (vertex - center).length());
	}
	return max_radius;
}

// Returns true if these segments do not intersect
static bool is_not_intersect(const pair<float, float>& segment_1, const pair<float, float>& segment_2)
{
	return segment_1.second < segment_2.first || segment_2.second < segment_1.first;
}

bool polygon::bulge_check() const
{
	auto prev_sign = sgn(
		get_edge(vertex_count() - 1).cross_product(get_edge(0)));

	for (size_t i = 0; i < vertex_count() - 1; ++i)
	{
		auto new_sign = sgn(
			get_edge(i).cross_product(get_edge(i + 1))
		);

		if (new_sign * prev_sign < 0)
		{
			return false;
		}

		prev_sign = new_sign;
	}
	return true;
}

//**PUBLIC METHODS**//

polygon::polygon(const vector_2d<float>& center, const std::initializer_list<vector_2d<float>> list, float angle)
	: shape(center),  rel_vertices_(list)
{
	for (const auto& rel_vertex : rel_vertices_)
	{
		if (abs(angle) > std::numeric_limits<float>::epsilon())
		{
			wc_vertices_.emplace_back(rel_vertex.get_rotated(angle) + center_);
		}
		else
		{
			wc_vertices_.emplace_back(rel_vertex + center_);
		}
	}
	
	radius_ = find_radius(wc_vertices_, center_);
	is_convex_ = bulge_check();
}

size_t polygon::vertex_count() const
{
	return rel_vertices_.size();
}

void polygon::draw(const drawer& drawer, bool draw_center)
{
	for (size_t i = 0; i < vertex_count() - 1; ++i)
	{
		drawer.draw_line(wc_vertices_[i], wc_vertices_[i + 1]);
	}
	drawer.draw_line(wc_vertices_.back(), wc_vertices_.front());

	if (draw_center)
	{
		drawer.draw_point(center_);
	}
}

void polygon::rotate(float angle)
{
	for (size_t i = 0; i < rel_vertices_.size(); ++i)
	{
		wc_vertices_[i] = rel_vertices_[i].get_rotated(angle) + center_;
	}
}

void polygon::move_to(const vector_2d<float>& position)
{
	

	for (size_t i = 0; i < rel_vertices_.size(); ++i)
	{
		wc_vertices_[i] = wc_vertices_[i] + position - center_;
	}

	center_ = position;
}

vector<vector_2d<float>> polygon::build_edges() const
{
	vector<vector_2d<float>> edges;

	for (size_t i = 0; i < vertex_count(); ++i)
	{
		edges.emplace_back(get_edge(i));
	}
	return edges;
}

bool polygon::is_convex() const
{
	return is_convex_;
}

vector_2d<float> polygon::get_edge(size_t index) const
{
	if (index == 0 || index >= wc_vertices_.size())
	{
		return wc_vertices_.front() - wc_vertices_.back();
	}

	return wc_vertices_[index] - wc_vertices_[index - 1];
}

std::pair<float, float> polygon::orthographic_projection(const vector_2d<float>& axis_vector) const
{
	vector<float> coordinates_on_axis;

	const auto axis_angle = axis_vector.angle();

	for (const auto& vertex : wc_vertices_)
	{
		coordinates_on_axis.emplace_back(vertex.get_rotated(-axis_angle).x);
	}

	auto min = *min_element(coordinates_on_axis.begin(), coordinates_on_axis.end());
	auto max = *max_element(coordinates_on_axis.begin(), coordinates_on_axis.end());

	return { min, max };
}

bool accurate_convex_collision_detect(const polygon& first, const polygon& second)
{
	if (!first.is_convex() || !second.is_convex())
	{
		throw std::bad_function_call();
	}

	for (size_t i = 0; i < first.vertex_count(); ++i)
	{
		auto edge = first.get_edge(i);

		auto first_polygon_projection = 
			first.orthographic_projection(edge.get_orthogonal());
		
		auto second_polygon_projection = 
			second.orthographic_projection(edge.get_orthogonal());

		if (is_not_intersect(first_polygon_projection, second_polygon_projection))
		{
			return false;
		}
	}

	for (size_t j = 0; j < second.vertex_count(); ++j)
	{
		auto edge = second.get_edge(j);

		auto first_polygon_projection = 
			first.orthographic_projection(edge.get_orthogonal());
		
		auto second_polygon_projection = 
			second.orthographic_projection(edge.get_orthogonal());

		if (is_not_intersect(first_polygon_projection, second_polygon_projection))
		{
			return false;
		}
	}
	return true;
}
