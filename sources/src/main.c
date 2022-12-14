/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL.h>
#include <time.h>

#include <constant.h>
#include <game.h>
#include <window.h>
#include <misc.h>
#include <menu.h>


int main(int argc, char *argv[]) {

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		error("Can't init SDL:  %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	srand(time(NULL));
	struct game* game = game_new();
	window_create(SIZE_BLOC * STATIC_MAP_WIDTH,
	SIZE_BLOC * STATIC_MAP_HEIGHT + 2*BANNER_HEIGHT + LINE_HEIGHT);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;
	int menu = 1;
	int load = 0;
	int difficulty=0;
	printf("p: pause\n s:save\n l:load\n z: take a door\n ");
	while (menu) {
		timer = SDL_GetTicks();
		if (menu==1){
			menu_display_menu();
			load = menu_update_menu();
			}
		if (load == 1)
			menu=0;
		if (load == 2){
			menu=2;
			menu_display_difficulty();
			difficulty = menu_update_difficulty();
			}
		if (difficulty!= 0)
			menu=0;
		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
	}

	// game loop
	// static time rate implementation

	game_set_difficulty(game , difficulty);
	game_set_monster(game ) ;

	if (load == 1)
		game_load(game);
	int win =0;
	int done = 0;
	while (!done) {
		timer = SDL_GetTicks();
		done = game_update(game, &done ,&win );
		game_display(game , done);

		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
	}

	game_free(game);

	SDL_Delay(3000);
	SDL_Quit();

	return EXIT_SUCCESS;
}
