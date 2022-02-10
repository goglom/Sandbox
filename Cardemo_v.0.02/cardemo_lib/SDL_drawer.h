#pragma once
#include "polygon.h"
#include "drawer.h"
#include "SDL_draw.h"

#include <SDL.h>
#include <exception>
#include <string>


struct SDL_Renderer_exception : std::exception
{
	SDL_Renderer_exception()
		: error_message_(SDL_GetError())
	{}
	[[nodiscard]] char const* what() const override
	{
		return error_message_.c_str();
	}

private:
	std::string error_message_;
}; 


class SDL_drawer final:
	public drawer
{
	SDL_Renderer*& renderer_;
	float scale_;
	color draw_color_;

public:
	explicit SDL_drawer(SDL_Renderer*& renderer, float scale = 1,
		const color& draw_color = colors::black)
		:
		renderer_(renderer), scale_(scale), draw_color_(draw_color)
	{}
	
	void draw_line(const vector_2d<float>& point_1,
		const vector_2d<float>& point_2) const override;
	
	void draw_point(const vector_2d<float>& point) const override;
	
	void set_draw_color(const color& color) override;
};