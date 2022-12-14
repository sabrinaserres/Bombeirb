/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>

struct player;

// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number);
void   player_free(struct player* player);

// Set the position of the player
void player_set_position(struct player *player, int x, int y);
// Set the key of player
void player_set_keys ( struct player* player);
void player_set_key( struct player* player , short x );
// test if the player own a key
int player_own_key( struct player* player , short index );

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);
void player_set_nb_bomb(struct player* player , int x );

// Set, Increase, Decrease the range of bomb that player can put
int  player_get_bomb_range(struct player * player);
void player_inc_bomb_range(struct player * player);
void player_dec_bomb_range(struct player * player);
void player_set_bomb_range(struct player* player,int x);



// Set, Increase, Decrease the number of player's hp
int player_get_hp(struct player* player);
void player_inc_hp(struct player* player);
void player_dec_hp(struct player* player);
void player_set_hp(struct player* player,int x );
int player_get_invincibility(struct player* player);
void player_set_invincibility(struct player* player);
// Move the player according to the current direction
int player_move(struct player* player, struct map* map);

// Display the player on the screen
void player_display(struct player* player);

#endif /* PLAYER_H_ */
