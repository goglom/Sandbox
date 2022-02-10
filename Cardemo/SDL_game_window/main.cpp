#include <iostream>

#include "SDL_game_window.h"
#include "SDL_drawer.h"
#include "car.h"
#include "car_player_control.h"
#include <numbers>


using namespace std;

constexpr float m_pi = numbers::pi_v<float>;

int main()
{
	SDL_game_window game_window(800, 400);
	if (!game_window.is_good())
	{
		cout << game_window.error_message();
		return 1;
	}

	bool quit = false;
	SDL_Event event;

	SDL_drawer drawer(game_window.get_renderer());
	car_player_control control;

	car car(
		{ 100.f, 100.f },
		colors::blue,
		colors::red,
		control,
		drawer
	);
	
	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		game_window.clear_screen();

		car.draw();
		
		game_window.update_render();
	}
	
	return 0;
}