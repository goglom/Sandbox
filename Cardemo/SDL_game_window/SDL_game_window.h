#pragma once


#include <SDL.h>
#undef main

#include <string>
#include "SDL_draw.h"


class SDL_game_window
{
	SDL_Window* game_window_;
	SDL_Renderer* game_renderer_;
	
	size_t screen_width_;
	size_t screen_height_;

	std::string error_message_;

	bool is_good_;

public:
	SDL_game_window(size_t width, size_t height);
	~SDL_game_window();

	bool is_good() const;
	const std::string& error_message() const;

	SDL_Renderer*& get_renderer();

	void update_render() const;
	void clear_screen() const;

	//bool load_media();
};

