#include <exception>
#include <string>

#include "SDL_game_window.h"

#include <iostream>

#include "SDL_draw.h"

using namespace std;

SDL_game_window::SDL_game_window(size_t width, size_t height)
	:	game_window_(nullptr),
		game_renderer_(nullptr),
		event_(),
		screen_width_(width),
		screen_height_(height),
		is_good_(true)
		
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		error_message_.append("SDL could not initialize! SDL Error: ");
		error_message_.append(SDL_GetError());
		is_good_ = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			error_message_.append("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		game_window_ = SDL_CreateWindow(
			"SDL Tutorial", 
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, 
			screen_width_, 
			screen_height_, 
			SDL_WINDOW_SHOWN
		);
		
		if (game_window_ == nullptr)
		{
			error_message_.append("Window could not be created! SDL Error: ");
			error_message_.append(SDL_GetError());
			is_good_ = false;
		}
		else
		{
			//Create renderer for window
			game_renderer_ = SDL_CreateRenderer(game_window_, -1, 
				SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			
			if (game_renderer_ == nullptr)
			{
				error_message_.append("Renderer could not be created! SDL Error: ");
				error_message_.append(SDL_GetError());
				is_good_ = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(game_renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
			}
		}
	}

	keyboard_state_ = SDL_GetKeyboardState(nullptr);
}

SDL_game_window::~SDL_game_window()
{
	SDL_DestroyRenderer(game_renderer_);
	SDL_DestroyWindow(game_window_);
	
	game_renderer_ = nullptr;
	game_window_ = nullptr;

	SDL_Quit();
}

bool SDL_game_window::is_good() const
{
	return is_good_;
}

const std::string& SDL_game_window::error_message() const
{
	return error_message_;
}

bool SDL_game_window::is_closed()
{
	while (SDL_PollEvent(&event_) != 0)
	{
		if (event_.type == SDL_QUIT)
		{
			return  true;
		}
	}
	return false;
}

bool SDL_game_window::quit_game_mode()
{
	while (SDL_PollEvent(&event_) != 0)
	{
		if (event_.type == SDL_KEYDOWN)
		{
			if (event_.key.keysym.sym == SDLK_ESCAPE)
				return  true;
		}
	}
	return false;
}

const Uint8* SDL_game_window::get_keyboard_state() const
{
	return keyboard_state_;
}

SDL_Renderer*& SDL_game_window::get_renderer()
{
	return game_renderer_;
}

void SDL_game_window::update_render() const
{
	SDL_RenderPresent(game_renderer_);
}

void SDL_game_window::clear_screen() const
{
	SDL_SetRenderDrawColor(game_renderer_, colors::white);
	SDL_RenderClear(game_renderer_);
}

int SDL_game_window::main_menu()
{
	cout << "Press key '1' or '2' to select test mode\n";

	bool quit = false;
	
	while (!quit)
	{
		while (SDL_PollEvent(&event_) != 0)
		{
			if (event_.type == SDL_KEYDOWN)
			{
				switch (event_.key.keysym.sym)
				{
				case SDLK_1:
					return mode_1;
				
				case SDLK_2 :
					return mode_2;
				}
			}
			if (event_.type == SDL_QUIT)
			{
				quit = true;
			}
		}
	}
	return escape;
}

std::pair<size_t, size_t> SDL_game_window::get_screen_resolution() const
{
	return { screen_width_, screen_height_ };
}

