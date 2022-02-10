#pragma once

#include <vector>
#include <list>

#include "vector_2d.h"
#include "drawer.h"

#define $$ [[nodiscard]]

class car_way final
{
	std::vector<std::pair<vector_2d<float>, float>> check_point_;
	drawer& drawer_;
	
public:

	car_way(
		std::initializer_list<std::pair<vector_2d<float>, float>> cp_list,
		drawer& drawer
	);
	
	$$ std::pair<vector_2d<float>, float> get_check_point(size_t index) const;
	$$ size_t get_cp_count() const;
	void draw_cp();
	
	
};

#undef $$