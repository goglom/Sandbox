#include "SDL_drawer.h"

void SDL_drawer::draw_line(const vector_2d<float>& point_1, const vector_2d<float>& point_2) const
{
	auto result = SDL_SetRenderDrawColor(renderer_, draw_color_);

	if (result != 0)
	{
		throw SDL_Renderer_exception();
	}

	result += SDL_RenderDrawLineF(
		renderer_,
		point_1.x,
		point_1.y,
		point_2.x,
		point_2.y
	);

	if (result != 0)
	{
		throw SDL_Renderer_exception();
	}
}

void SDL_drawer::draw_point(const vector_2d<float>& point) const
{
	auto result = SDL_SetRenderDrawColor(renderer_, draw_color_);

	if (result != 0)
	{
		throw SDL_Renderer_exception();
	}

	result += SDL_RenderDrawPointF(renderer_, point.x, point.y);

	if (result != 0)
	{
		throw SDL_Renderer_exception();
	}
}

void SDL_drawer::set_draw_color(const SDL_Color& color)
{
	draw_color_ = color;
}
