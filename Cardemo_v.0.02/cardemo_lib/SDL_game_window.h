#pragma once


#include <SDL.h>
#undef main

#include <string>

#define $$ [[nodiscard]]

enum main_menu_result
{
	mode_1,
	mode_2,
	escape
};

class SDL_game_window final
{
	SDL_Window* game_window_;
	SDL_Renderer* game_renderer_;
	SDL_Event event_;

	const Uint8* keyboard_state_;
	
	size_t screen_width_;
	size_t screen_height_;

	std::string error_message_;

	bool is_good_;

public:
	SDL_game_window(size_t width, size_t height);
	~SDL_game_window();

	// Returns true if window was create successfully
	$$ bool is_good() const;

	// Returns error message if an error happened
	$$ const std::string& error_message() const;

	// Returns true if game window closing was calling by user
	$$ bool is_closed();

	// Returns true if closing of game mode was calling by user
	bool quit_game_mode();

	// Returns array with keyboard state
	$$ const Uint8* get_keyboard_state() const;
	
	SDL_Renderer*& get_renderer();

	// Update window renderer
	void update_render() const;

	// Clear window renderer
	void clear_screen() const;

	int main_menu();


	// First value - width, second value - height
	$$ std::pair<size_t, size_t> get_screen_resolution() const;

	//bool load_media();
};

#undef $$