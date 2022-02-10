#include "car_way.h"
#include "SDL_draw.h"

car_way::car_way(std::initializer_list<std::pair<vector_2d<float>, float>> cp_list, drawer& drawer)
	: check_point_(cp_list), drawer_(drawer)
{}

std::pair<vector_2d<float>, float> car_way::get_check_point(size_t index) const
{
	return check_point_[index];
}

size_t car_way::get_cp_count() const
{
	return check_point_.size();
}

void car_way::draw_cp()
{
	for (auto & [cp, radius] : check_point_)
	{
		drawer_.set_draw_color(colors::black); //Black Color Matter
		drawer_.draw_point(cp);
	}
}
