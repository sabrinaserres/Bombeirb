/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>

struct map {
	int width;
	int height;
	unsigned char* grid;
};

#define CELL(i,j) ( (i) + (j) * map->width)

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;

}

//Test if a box can be move and if possible move it
int map_move_box(struct map* map , int x , int y , int direction ){
	if ( map_get_cell_type( map, x, y ) == CELL_BOX){
		switch (direction) {
		case NORTH:
			if (map_is_inside( map, x, y - 1)) {
				if (!map_get_cell_type( map, x, y - 1)){
					map_set_cell_type (map, x ,y-1 , map_get_cell_type(map ,x ,y));
					map_set_cell_type (map , x ,y,CELL_EMPTY);
					return 1;
				}
			}

			break;

		case SOUTH:
			if (map_is_inside( map, x, y + 1)){
				if (!map_get_cell_type(map, x, y + 1)){
					map_set_cell_type (map, x ,y+1 , map_get_cell_type(map ,x ,y));
					map_set_cell_type (map , x ,y,CELL_EMPTY);
					return 1;
				}
			}
			break;

		case WEST:
		if (map_is_inside( map, x-1, y )){
			if (!map_get_cell_type(map, x-1, y)){
				map_set_cell_type (map, x-1 ,y , map_get_cell_type(map ,x ,y));
				map_set_cell_type (map , x ,y,CELL_EMPTY);
				return 1;
			}
		}
			break;

		case EAST:
			if (map_is_inside( map, x+1, y)){
				if (!map_get_cell_type(map, x+1, y)){
					map_set_cell_type (map, x+1 ,y , map_get_cell_type(map ,x ,y));
					map_set_cell_type (map , x ,y,CELL_EMPTY);
					return 1;
					}
				}
			break;
		}
	}
	return 0 ;
}

// retourne False si les nouvelles coordonnées du joueur sont hors le la map.
int map_is_inside(struct map* map, int x, int y)
{
	assert(map);
	if( x<0 || x> (map_get_width(map) -1 ))
		return 0;
	if ( y< 0 || y> (map_get_height(map) -1) )
		return 0;
	return 1;
}


void map_free(struct map *map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0;
}

char map_get_cell(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] ;
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type){
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void map_set_cell (struct map* map, int x, int y, int cell  ){
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = cell;
}

void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;
	}
}

void display_door(struct map* map, int x, int y, unsigned char type)
{
	switch (type & 0xf0){
		case DOOR_OPEN :
		window_display_image(sprite_get_door_opened(),x ,x);
		break;
		case DOOR_CLOSED :
		window_display_image(sprite_get_door_closed(), x, y);
		break;
	}
}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	}
}





void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;
	    unsigned char type = map->grid[CELL(i,j)];
	    switch (type & 0xf0) {
				case CELL_SCENERY:
					display_scenery(map, x, y, type);
					break;
					case CELL_BOX:
						window_display_image(sprite_get_box(), x, y);
						break;
					case CELL_BONUS:
						display_bonus(map, x, y, type);
						break;
					case CELL_KEY:
						window_display_image(sprite_get_key(), x, y);
						break;
					case CELL_DOOR:
						if (((map->grid[CELL(i,j)])& 0x01) == 1)
							window_display_image(sprite_get_door_opened(), x, y);
						else
							window_display_image(sprite_get_door_closed(), x, y);
						break;
					case CELL_BOMB:
					window_display_image(sprite_get_bomb(map_get_cell(map,i,j) & 0x0f), x, y);

					//window_display_image(sprite_get_bomb(4), x, y);
					break;
	    }
	  }
	}
}

struct map* map_get_static(void)
{
	struct map* map = map_new(STATIC_MAP_WIDTH, STATIC_MAP_HEIGHT);

	unsigned char themap[STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT] = {
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, 0x54, 0x52, 0x51, 0x53, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
	  CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BONUS, CELL_EMPTY,
	  CELL_BOX, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_BONUS, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_BOX, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_BOX_LIFE, CELL_EMPTY,
	  CELL_BOX,  CELL_EMPTY, CELL_DOOR, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
		};

	for (int i = 0; i < STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT; i++)
		map->grid[i] = themap[i];

	return map;
}

void map_set_maps(struct map ** maps  , short difficulty, short levels , int load){
	for ( int i = 0; i<levels ; i++){
		maps[i]=map_get_map(difficulty, i, load);
	}
}


struct map* map_get_map(short difficulty , short level ,int load ){
	int width = 0;
	int height = 0;
	int cell = 0;
	char dest[60];
	if (load==0){
		sprintf(dest,"./data/origin_data/difficulty_%d/level_%d.txt",difficulty,level);
	}
	else {
		sprintf(dest,"./data/save/level_%d.txt",level);
	}
	FILE* fichier = fopen(dest,"r");
	fscanf(fichier ,"%i_%i",&width,&height);
	struct map* map = map_new(width, height);
	for (int i = 0; i < width * height; i++){
		fscanf(fichier , "%i_",&cell);
		map->grid[i] = cell;
	}
	map->height = height ;
	map->width = width;
	fclose(fichier);
	return map;
}

void map_save_map(struct map* map , int index ){
		int width = map->width;
		int height = map->height;
		char dest[40];
		sprintf(dest,"./data/save/level_%d.txt",index);
		FILE* fichier = fopen(dest,"w");
		fprintf( fichier , "%i_%i \n",width , height);
		for (int i = 0; i < width * height; i++){
			if (i%width ==0)
				fprintf( fichier , "\n");
			fprintf(fichier , "%i_",(map->grid[i]));
		}
		fclose(fichier);
}
