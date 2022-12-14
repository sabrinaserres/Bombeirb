

#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <menu.h>




int input_menu_keyboard() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
        //new game
			case SDLK_a :
				return 2;
				break;
        //load the game
			case SDLK_z :
				return 1;
				break;
			default:
				break;
			}

			break;
		}
	}
	return 0;
}


int input_difficulty_keyboard() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
        //new game
			case SDLK_a :
				return 1;
				break;
        //load the game
			case SDLK_z :
				return 2;
				break;
      case SDLK_e:
  			return 3;
  			break;
			default:
				break;
			}
			break;
		}
	}
	return 0;
}



int menu_update_menu(){
	return(input_menu_keyboard());
}
int menu_update_difficulty(){
  return(input_difficulty_keyboard());
}

void menu_display_menu(){
	window_clear();
  window_display_image(sprite_get_menu(),0,0);
	window_refresh();
}

void menu_display_difficulty(){
	window_clear();
  window_display_image(sprite_get_difficulty(),0,0);
	window_refresh();
}
