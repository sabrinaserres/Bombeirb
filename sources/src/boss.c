
#include <SDL/SDL_image.h>
#include <assert.h>

#include <constant.h>
#include <boss.h>
#include <map.h>
#include <window.h>
#include <game.h>
#include <player.h>
#include <monster.h>
#include <sprite.h>

struct boss {
	int x, y;
	enum direction direction;
	int hp ;
	int last_move;
	int live;
	int first_move;
	int first_move_timer;
};

struct boss * boss_init(int difficulty){
	struct boss * boss = malloc	( sizeof(struct boss));
	boss-> x = 11;
	boss->y =11;
	boss->direction = NORTH;
	boss->hp = difficulty*3;
	boss->last_move=SDL_GetTicks();
	boss->live=1;
	boss->first_move = 1;
	boss->first_move_timer = SDL_GetTicks();
	return boss;
}

void boss_update(struct boss* boss ,struct monster**monster, struct player* player , struct map * map 	, int level,int difficulty){
	if (boss->live==2){
		boss->live = 0;
	}
	if ((level==7 )&&((SDL_GetTicks() - boss->last_move)>(2000/(difficulty*7)) )&& (boss->live==1)) {
		boss->direction =boss_next_move(map , boss , player_get_x(player),player_get_y(player));
		boss_move(boss, map);
		if ((boss->x )==(player_get_x(player))&& ((boss->y)==(player_get_y(player))))
			player_dec_hp(player);
		boss->last_move = SDL_GetTicks();
		if (boss->first_move){
			boss->first_move=0;
			boss->first_move_timer =SDL_GetTicks();
		}
	}
}


//Free
void boss_free(struct boss* boss ) {
  assert(boss);
	free(boss);
}
// POSITION

int boss_get_x( struct boss * boss){
	assert(boss);
	return (boss->x);
}


int boss_get_y( struct boss * boss){
	assert(boss);
	return (boss->y);
}

void boss_set_x( struct boss * boss , int x ){
	assert(boss);
	((boss)->x) = x;
}


void boss_set_y( struct boss * boss , int y){
	assert(boss);
	((boss)->y) = y;
}

void boss_set_position( struct boss * boss ,int x , int y ){
	assert(boss);
	(boss->x) = x;
	(boss->y) = y;
}


// direction

void boss_set_current_way(struct boss * boss , enum direction way) {
	assert(boss);
	(boss->direction) = way;
}

//HP
int boss_get_hp(struct boss * boss ) {
	assert(boss);
	return ( (boss->hp))  ;
}

void boss_inc_hp(struct boss * boss ) {
	assert(boss);
	(boss->hp) +=1;
}

void boss_dec_hp(struct boss* boss) {
	assert(boss);
	boss->hp -= 1 ;
	if (boss->hp == 0)
		boss->live =2;
}


void boss_set_hp(struct boss* boss,int x ) {
	assert(boss);
	(boss->hp) = x;
}

// move
void boss_move(struct boss* boss, struct map* map) {
	switch (boss->direction) {
		case NORTH:{
				map_set_cell_type(map,boss->x,boss->y,CELL_EMPTY);
				boss->y--;
				map_set_cell_type(map,boss->x,boss->y,(CELL_MONSTER|0x0f));
				break;}
		case SOUTH:{
				map_set_cell_type(map,boss->x,boss->y,CELL_EMPTY);
				boss->y++;
				map_set_cell_type(map,boss->x,boss->y,(CELL_MONSTER|0x0f));
				break;}

		case WEST:{
				map_set_cell_type(map,boss->x,boss->y,CELL_EMPTY);
				boss->x--;
				map_set_cell_type(map,boss->x,boss->y,(CELL_MONSTER|0x0f));
				break;}

		case EAST:{
				map_set_cell_type(map,boss->x,boss->y,CELL_EMPTY);
				boss->x++;
				map_set_cell_type(map,boss->x,boss->y,(CELL_MONSTER|0x0f));
				break;}
		default:
			break ;
		}
}
//Next move
enum direction boss_next_move( struct map* map  ,struct boss* boss ,int x_player, int y_player){
	int x_b = boss->x;
	int y_b =boss->y;
	enum direction dir;
  int valide=0;
		if ((x_b-x_player)==0){
			switch ((y_b-y_player)/abs(y_b-y_player)) {
					case 1:
	          valide = boss_next_move_valid(map,x_b,y_b,NORTH);
	          if (valide){
	            dir = NORTH;
	            break;}
				  case -1:
	          valide = boss_next_move_valid(map,x_b,y_b,SOUTH);
	          if (valide){
	            dir = SOUTH;
	            break;}
	        default :
	          break;
	    }
		}
    if(valide)
	   return dir;
    switch ((x_b-x_player)/abs(x_b-x_player)) {
 			case 1:
         valide = boss_next_move_valid(map,x_b,y_b,WEST);
         if (valide){
           dir = WEST;
           break;}
 		  case -1:
         valide = boss_next_move_valid(map,x_b,y_b,EAST);
         if (valide){
           dir = EAST;
           break;}
       default :
         break;
     }
     return dir ;
}


enum direction boss_next_move_valid( struct map* map  ,int x , int y , enum direction dir ){
	int valide = 0;
	switch (dir) {
			case NORTH:
					if(map_is_inside(map,x,y-1 )){
						if ((map_get_cell_type(map,x,y-1) == CELL_EMPTY)|| (map_get_cell_type(map ,x,y-1) == CELL_BONUS) ||((map_get_cell_type(map ,x ,y-1 ) == CELL_BOX ) && map_move_box (map , x ,y-1 ,dir))){
							valide= 1 ;
						}
					}
					break;
				case WEST:
					if(map_is_inside(map,x-1,y )){
						if ((map_get_cell_type(map,x-1,y) == CELL_EMPTY)|| (map_get_cell_type(map ,x-1,y) == CELL_BONUS) ||((map_get_cell_type(map ,x -1,y ) == CELL_BOX ) && map_move_box (map , x-1 ,y ,dir))){
							valide= 1 ;
						}
					}
					break;
			case SOUTH:
				if(map_is_inside(map,x,y+1 )){
					if ((map_get_cell_type(map,x,y+1) == CELL_EMPTY)|| (map_get_cell_type(map ,x,y+1) == CELL_BONUS) ||((map_get_cell_type(map ,x ,y+1 ) == CELL_BOX ) && map_move_box (map , x ,y+1 ,dir))){
						valide= 1 ;
					}
				}
				break;
			case EAST:
			if(map_is_inside(map,x+1,y )){
					if ((map_get_cell_type(map,x+1,y) == CELL_EMPTY)|| (map_get_cell_type(map ,x+1,y) == CELL_BONUS) ||((map_get_cell_type(map ,x +1,y ) == CELL_BOX ) && map_move_box (map , x+1 ,y ,dir))){
						valide=1 ;
					}
				}
			break;
			default:
				valide =1;
			break;
	}
	return valide;
}

void display_boss(struct boss* boss){
	assert(boss);
	if (boss->live)
		window_display_image(sprite_get_monster(boss->direction), boss->x * SIZE_BLOC, boss->y * SIZE_BLOC);
}
