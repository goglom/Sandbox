#include <iostream>

#include "SDL_game_window.h"
#include "tests.h"

using namespace std;


int main()
{
	SDL_game_window game_window(960, 640);
	if (!game_window.is_good())
	{
		cout << game_window.error_message();
		return 1;
	}
	
	while (true)
	{
		switch (game_window.main_menu())
		{
		case mode_1:
			test_mode_1(game_window);
			break;

		case mode_2:
			test_mode_2(game_window);
			break;

		case escape:
				return 0;
		}
	}
}
