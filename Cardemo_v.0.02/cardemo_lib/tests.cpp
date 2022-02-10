#include <iostream>
#include <numbers>



#include "bot_car_control.h"
#include "car.h"
#include "car_way.h"
#include "game_scene.h"
#include "SDL_car_player_control.h"
#include "SDL_draw.h"
#include "SDL_drawer.h"
#include "SDL_game_window.h"
#include "trace_barrier.h"
using namespace std;

constexpr float m_pi = numbers::pi_v<float>;



void test_mode_1(SDL_game_window& game_window)
{
	cout << "Test mode 1 (player (use keys: up, down, right, left - arrows to steer and space to brake)).\n Press 'ESC' to quit at main menu\n";

	auto [width_, height_] = game_window.get_screen_resolution();
	const float width = width_;
	const float height = height_;
	const float scale = 10;

	SDL_drawer drawer(game_window.get_renderer(), scale);

	car_way car_way(
		{
			{{10, 30}, 5},
			{{30, 10}, 5},
			{{60, 10}, 5},
			{{80, 30}, 5},
			{{60, 50}, 5},
			{{30, 50}, 5}
		},
		drawer
	);

	SDL_car_player_control player_1(
		game_window.get_keyboard_state(),
		car_way,
		SDL_SCANCODE_UP,
		SDL_SCANCODE_DOWN,
		SDL_SCANCODE_RIGHT,
		SDL_SCANCODE_LEFT,
		SDL_SCANCODE_SPACE
	);

	game_scene scene(
		{
			new trace_barrier(
				rectangle({15, 5}, 25.f, 10.f, -0.5f),
				drawer,
				colors::black,
				0.5f,
				0.5f
			),
			new trace_barrier(
				rectangle({0, 0.5f * height / scale}, 10, height / scale, 0.f),
				drawer,
				colors::green,
				0.5f,
				1.f
			),
			new trace_barrier(
				rectangle({50, 0}, 80, 10, 0.f),
				drawer,
				colors::blue,
				1.f,
				1.f
			),
			new trace_barrier(
				rectangle({0.5f * width / scale, height / scale},
					width / scale, 10, 0.f),
				drawer,
				colors::blue,
				1.f,
				1.f
			),
			new trace_barrier(
				polygon({85, 20}, {{10, 50} , {10, -18}, {-1, -15}}),
				drawer,
				colors::blue,
				1.f,
				1.f
			),
			new trace_barrier(
				rectangle({0.5f * width / scale, 0.5f * height / scale},
					20, 10, 0.f),
				drawer,
				colors::black,
				0.1f,
				0.1f
			),
		},
		{
			new car(
				{ 10.f, 30.f },
				-m_pi / 2.f,
				colors::red,
				colors::black,
				player_1,
				drawer
			)
		},
		car_way
	);

	scene.turn_drawing_check_points();

	auto prev_round = player_1.get_rounds();
	
	while (!game_window.quit_game_mode())
	{
		game_window.clear_screen();

		scene.collision_process();
		scene.move_process();
		scene.draw_scene();

		auto cur_round = player_1.get_rounds();

		if (cur_round != prev_round)
		{
			cout << "round: " << cur_round << "\n";
			prev_round = cur_round;
		}

		game_window.update_render();
	}
	game_window.clear_screen();
	game_window.update_render();
}

void test_mode_2(SDL_game_window& game_window)
{
	cout << "Test mode 2 (bot). Press 'ESC' to quit at main menu\n";

	auto [width_, height_] = game_window.get_screen_resolution();
	const float width = width_;
	const float height = height_;
	const float scale = 10;

	SDL_drawer drawer(game_window.get_renderer(), scale);

	car_way car_way(
		{
			{{15, 30}, 6},
			{{30, 10}, 6},
			{{60, 10}, 6},
			{{80, 30}, 6},
			{{60, 50}, 6},
			{{30, 50}, 6}
		},
		drawer
	);

	bot_car_control bot(car_way);

	game_scene scene(
		{
			new trace_barrier(
				rectangle({15, 2}, 25.f, 10.f, -0.5f),
				drawer,
				colors::black,
				0.5f,
				0.5f
			),
			new trace_barrier(
				rectangle({0, 0.5f * height / scale}, 10, height / scale, 0.f),
				drawer,
				colors::green,
				0.9f,
				1.f
			),
			new trace_barrier(
				rectangle({50, 0}, 80, 10, 0.f),
				drawer,
				colors::blue,
				1.f,
				1.f
			),
			new trace_barrier(
				rectangle({0.5f * width / scale, height / scale},
					width / scale, 10, 0.f),
				drawer,
				colors::blue,
				1.f,
				1.f
			),
			new trace_barrier(
				polygon({85, 20}, {{10, 50} , {10, -18}, {-1, -15}}),
				drawer,
				colors::blue,
				1.f,
				1.f
			)
		},
		{
			new car(
				{ 10.f, 30.f },
				-m_pi / 2.f,
				colors::blue,
				colors::black,
				bot,
				drawer
			)
		},
		car_way
	);

	scene.turn_drawing_check_points();

	auto prev_round = bot.get_rounds();

	while (!game_window.quit_game_mode())
	{
		game_window.clear_screen();

		scene.collision_process();
		scene.move_process();
		scene.draw_scene();

		const auto cur_round = bot.get_rounds();

		if (cur_round != prev_round)
		{
			cout << "round: " << cur_round << "\n";
			prev_round = cur_round;
		}

		game_window.update_render();
	}
	game_window.clear_screen();
	game_window.update_render();
}