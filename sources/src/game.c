/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb.h>
#include <monster.h>
#include <boss.h>

struct game {
	struct map** maps;       // the game's map
	int difficulty;
	int levels;        // nb maps of the game
	int level;
	struct player* player;
	struct bomb **bombs;
	int nb_monsters;
	struct monster** monsters;
	struct boss* boss;
};

struct game* game_new(void) {
	sprite_load(); // load sprites into process memory
	struct game* game = malloc(sizeof(*game));
	game->maps = malloc(sizeof(struct game));
	game->difficulty = 1;
	game->levels = 8;
	map_set_maps(game->maps  , game->difficulty, game->levels , 0);
	game->level = 0;
	game->player = player_init(4);
	// Set default location of the player
	player_set_position(game->player, 0, 0);
	player_set_keys ( game->player);
	game->bombs = bombs_init();
	game->nb_monsters=0;
	game ->monsters = monsters_init(&(game->nb_monsters) , game->difficulty);
	game->boss = boss_init(game->difficulty);
	return game;
}

void game_set_difficulty(struct game*game , int difficulty){
	assert(game);
	game->difficulty = difficulty;
}
void game_set_monster(struct game * game ){
		assert(game);
		(game->monsters)=monsters_init(&(game->nb_monsters) , game->difficulty);
	}
void game_set_boss(struct game*game ){
			assert(game);
			boss_free(game->boss);
			(game->boss)=boss_init(game->difficulty);
	}


void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
	monsters_free(game->monsters , game->nb_monsters);
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->level];
}

struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}



void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	for (int i =0 ; i<9;i++){
		x = 0.6*i*white_bloc + 0.6*i* SIZE_BLOC + 0.5;
		window_display_image(sprite_get_key(), x, y);
	}
	x = 1* SIZE_BLOC;
	y = ((map_get_height(map)+1) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_hp(game_get_player(game))), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_bomb_range(game_get_player(game))), x, y);
}

void game_display(struct game* game , int done) {
	assert(game);
	if (!done ){
		window_clear();
		game_banner_display(game);
		map_display(game_get_current_map(game));
		display_explosion(game->bombs,game->maps[game->level],player_get_bomb_range(game->player),game->level);
		player_display(game->player);
		monsters_display(game->monsters , game->nb_monsters, game->level);
		if (game->level == 7)
			display_boss(game->boss);
		window_refresh();
	}
}

static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);


	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				player_move(player, map);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(player, map);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(player, map);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(player, map);
				break;
			case SDLK_SPACE:

				if ((player_get_nb_bomb(player)>0) && (map_get_cell_type(map, player_get_x(player), player_get_y(player))==CELL_EMPTY))
				{

					game->bombs = bomb_init(game->bombs,game->maps[game->level] ,player_get_x(player), player_get_y(player),game->level);

					player_dec_nb_bomb(player);


					return 0;

				}
				break;
			case SDLK_z:
				if((map_get_cell_type(map, player_get_x(player), player_get_y(player))) == CELL_DOOR){
					short new_level = ((map_get_cell(map, player_get_x(player), player_get_y(player))&0x0f)>>1);
					if (player_own_key( player , new_level)){
						int porte = ((map_get_cell(map, player_get_x(player), player_get_y(player))) | 0x01 );
						map_set_cell ( map , player_get_x(player), player_get_y(player) , porte );
						game->level=new_level;
						map = game_get_current_map(game);
						porte = ((map_get_cell(map, player_get_x(player), player_get_y(player))) | 0x01 );
						map_set_cell ( map , player_get_x(player), player_get_y(player) , porte );

					}
				}
				break;
			case SDLK_p :
				while (1){
					SDL_Event event;
					SDL_PollEvent(&event);
					if((event.type)==SDL_KEYDOWN) {
						if((event.key.keysym.sym)==SDLK_p) {
							break;
						}
					}
				}
				break;
			//ajouter les bombes et les montres a save et load
			case SDLK_s :
				game_save(game);
				break;
			case SDLK_l :
				game_load(game);
				break;
			default:
				break;
			}

			break;
		}
	}
	return 0;
}


int game_update(struct game* game , int* done , int * win){
	if ( player_get_hp(game->player)==0){
		*done = 1;
		*win =-1;
	}
	if ( boss_get_hp(game->boss)==0){
		*done = 1;
		*win =1;
	}

	bombs_update(game->bombs,game->maps,game->monsters, game->player,game -> boss ,player_get_bomb_range(game->player),game->level );
	monster_update(game->monsters, game->nb_monsters,game->player,game->maps[game->level] ,game->level, game->difficulty);
	if(game->level == 7) {
		boss_update(game->boss , game->monsters , game->player,game->maps[game->level], game->level , game -> difficulty );
	}
	if(input_keyboard(game))
		return 1; // exit game

	return 0;
}


void game_save(struct game* game){
	FILE* fichier = fopen("./data/save/Save.txt","w");
	fprintf( fichier , "%i ",(game->difficulty));
	fprintf( fichier , "%i ",(game->levels));
	fprintf( fichier , "%i ",(game->level));
	fprintf( fichier , "%hi %hi %hi %hi %hi ",player_get_x(game->player), player_get_y(game->player), player_get_nb_bomb(game->player),player_get_bomb_range(game->player), player_get_hp(game->player));
	for(int i = 0; i< (game->levels); i++){
		fprintf( fichier , "%i ",player_own_key((game->player),i));
	}
	fclose(fichier);
	for(int i = 0; i<(game->levels); i++){
		map_save_map((game->maps)[i] , i );
	}
	monster_save_monster(game->monsters , game->nb_monsters);
	bomb_save_bomb(game->bombs);
}

void game_load(struct game* game){
	FILE* fichier = fopen("./data/save/Save.txt","r");
	fscanf( fichier , "%i ",&(game->difficulty));
	fscanf( fichier , "%i ",&(game->levels));
	fscanf( fichier , "%i ",&(game->level));
	int x;
	int y;
	int bomb ;
	int bomb_range;
	int hp;
	fscanf( fichier , "%i %i %i %i %i ",&x ,&y ,&bomb,&bomb_range, &hp );
	player_set_position(game->player , x, y);
	player_set_nb_bomb(game->player , bomb);
	player_set_bomb_range(game->player , bomb_range);
	player_set_hp(game->player,hp);
	for(int i = 0; i< (game->levels); i++){
		int key = 0;
		fscanf( fichier , "%i ",&key);
		if (key ==1 )
		player_set_key((game->player),i);
	}
	fclose(fichier);
	map_set_maps(game->maps ,0, game->levels, 1 );
	monster_load_monster(game->monsters , &(game->nb_monsters));
	bomb_load_bomb(game->bombs);
}
