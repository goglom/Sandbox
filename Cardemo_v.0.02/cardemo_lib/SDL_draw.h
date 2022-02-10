#pragma once
#include <SDL.h>
#include "drawer.h"

namespace colors
{
	constexpr color white = { 0xff, 0xff, 0xff, 0xff };
	constexpr color black = { 0x00, 0x00, 0x00, 0xff };
	constexpr color red = { 0xff, 0x00, 0x00, 0xff };
	constexpr color green = { 0x00, 0xff, 0x00, 0xff };
	constexpr color blue = { 0x00, 0x00, 0xff, 0xff };
	constexpr color yellow = { 0xff, 0xff, 0x00, 0xff };

}

// Adaptation SDL function to color struct
inline int SDL_SetRenderDrawColor(SDL_Renderer* renderer, const color& color)
{
	return SDL_SetRenderDrawColor(
		renderer,
		color.red,
		color.green,
		color.blue,
		color.alpha
	);
}

// Adaptation SDL function to vector_2d class
inline int SDL_RenderDrawLineF(SDL_Renderer* renderer, const vector_2d<float>& point_1, 
	const vector_2d<float>& point_2)
{
	return SDL_RenderDrawLineF(
		renderer,
		point_1.x, point_1.y,
		point_2.x, point_2.y
	);
}