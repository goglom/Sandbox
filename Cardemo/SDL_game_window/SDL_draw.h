#pragma once
#include <SDL.h>

namespace colors
{
	constexpr SDL_Color white = { 0xff, 0xff, 0xff, 0xff };
	constexpr SDL_Color black = { 0x00, 0x00, 0x00, 0xff };
	constexpr SDL_Color red = { 0xff, 0x00, 0x00, 0xff };
	constexpr SDL_Color green = { 0x00, 0xff, 0x00, 0xff };
	constexpr SDL_Color blue = { 0x00, 0x00, 0xff, 0xff };
}

inline int SDL_SetRenderDrawColor(SDL_Renderer* renderer, const SDL_Color& color)
{
	return SDL_SetRenderDrawColor(
		renderer,
		color.r,
		color.g,
		color.b,
		color.a
	);
}
