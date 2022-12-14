/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>

struct player {
	int x, y;
	enum direction direction;
	int bombs;
	int bombs_range;
	int hp ;
	int invincibility; 	// time of the last time the player recieved a hit
	short* keys;
};

struct player* player_init(int bombs) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->direction = NORTH;
	player->bombs = bombs;
	player->hp = 3;
	player ->bombs_range = 3;
	player->keys=malloc(sizeof(short)*8);
	return player;
}

//KEY
void player_set_keys ( struct player* player){
	for (int i=0 ; i<8 ; i++ ){
		player->keys[i]=0;
	}
}

int player_own_key( struct player* player , short index ){
	return (player ->keys [index]);
}

void player_set_key( struct player* player , short x ){
	(player ->keys[ x])=1;

}


// POSITION :

void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}


void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->direction = way;
}

// NB BOMBS :

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->bombs;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player-> bombs += 1 ;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->bombs -= 1;
}

void player_set_nb_bomb(struct player* player , int x ) {
	assert(player);
	player->bombs = x;
}


// RANGE BOMBS :
int player_get_bomb_range(struct player* player) {
	assert(player);
	return player-> bombs_range;
}

void player_inc_bomb_range(struct player* player) {
	assert(player);
	player->bombs_range += 1;
}

void player_dec_bomb_range(struct player* player) {
	assert(player);
		if (player_get_bomb_range( player )>1)
		player->bombs_range -=1  ;
}

void player_set_bomb_range(struct player* player,int x) {
	assert(player);
	player->bombs_range = x  ;
}

// HP:

int player_get_hp(struct player* player) {
	assert(player);
	return ( (player->hp))  ;
}

void player_inc_hp(struct player* player) {
	assert(player);
	player->hp = (player->hp) +1;
}

void player_dec_hp(struct player* player) {
	assert(player);
	player->hp = (player->hp)- 1 ;
}

void player_set_hp(struct player* player,int x ) {
	assert(player);
	player->hp = x;
}

int player_get_invincibility(struct player* player) {
	assert(player);
	return ( (player->invincibility));
}

void player_set_invincibility(struct player* player){
	assert(player);
	if((SDL_GetTicks()-player_get_invincibility(player))>1000){
		player->invincibility = SDL_GetTicks();
	}

}

//


static int player_move_aux(struct player* player, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y) ) {
	case CELL_SCENERY:

		return 0;
		break;

	case CELL_BOX:
		return map_move_box(map,x , y, player->direction );
		break;

	case CELL_BONUS:
		switch ( map_get_cell(map, x, y)& 0x0f ) {
			case BONUS_BOMB_NB_DEC:
				player_dec_nb_bomb(player);
				break;
			case BONUS_BOMB_NB_INC:
				player_inc_nb_bomb(player);
				break;
			case BONUS_BOMB_RANGE_DEC:
				player_dec_bomb_range(player);
				break;

			case BONUS_BOMB_RANGE_INC:
				player_inc_bomb_range(player);
				break;
			case BONUS_LIFE:
				player_inc_hp(player);
				break;
			default:
				break;
		}
		map_set_cell_type(map,x,y,CELL_EMPTY);
		return 1 ;
		break;
	case CELL_KEY:{
		int key=map_get_cell(map, x, y)& 0x0f;
		player->keys[key]=1;
		map_set_cell_type(map, x, y, CELL_EMPTY);
		return 1 ;
		break;
		}
	case CELL_BOMB :
		return 0;
		break;

	case CELL_MONSTER:{
		int hit_timer = SDL_GetTicks();
		if ((hit_timer-(player->invincibility))>2000){
			player_dec_hp(player);
			player->invincibility= hit_timer;
		}
		return 0;

		break;}

	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->direction) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1)) {
			player->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1)) {
			player->y++;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y)) {
			player->x--;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y)) {
			player->x++;
			move = 1;
		}
		break;
	}


	if (move) {
		//map_set_cell_type(map, x, y, CELL_EMPTY);
	}
	return (move);
	//return 1;
}


void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}
