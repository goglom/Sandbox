#include <exception>
#include <string>

#include "SDL_game_window.h"

using namespace std;

SDL_game_window::SDL_game_window(size_t width, size_t height)
	: game_window_(nullptr),
	  game_renderer_(nullptr),
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
			game_renderer_ = SDL_CreateRenderer(game_window_, -1, SDL_RENDERER_ACCELERATED);
			
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

