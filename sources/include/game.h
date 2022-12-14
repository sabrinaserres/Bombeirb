/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <map.h>
#include <monster.h>


// Abstract data type
struct game;

// Create a new game
struct game* game_new();


void game_set_difficulty(struct game*game ,int difficulty);
void game_set_monster(struct game * game );

// Free a game
void game_free(struct game* game);

// Return the player of the current game
struct player* game_get_player(struct game* game);

// Return the current map
struct map* game_get_current_map(struct game* game);

// Display the game on the screen
void game_display(struct game* game, int done );

// update
int game_update(struct game* game , int * done , int * win);


void game_load(struct game* game);
void game_save(struct game* game);
#endif /* GAME_H_ */
