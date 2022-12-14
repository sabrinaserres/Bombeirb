#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <map.h>
#include <game.h>
#include <monster.h>

struct monster {
	int id;
	int x, y;
	enum direction direction;
	int level;
	int hp ;
	int owned_object;
	int last_move;
	int live;
};

struct monster** monsters_init( int * nb_monsters , int difficulty) {
	int monsters_nb=0;
	char dest[60];
	int x , y ,hp,level , owned_object;
	sprintf(dest,"./data/origin_data/difficulty_%d/monsters.txt",difficulty);
	FILE* fichier = fopen(dest,"r");
	fscanf(fichier ,"%i",&monsters_nb);
	*nb_monsters = monsters_nb;
	struct monster** monsters = malloc(sizeof(struct monsters *)*monsters_nb);
	for (int i = 0; i < monsters_nb; i++){
		monsters[i]=malloc(sizeof(struct monster));
		fscanf( fichier ,"%i_%i_%i_%i_%i_", &x , &y  , &level , &hp, &owned_object );
		monsters[i]->id = i;
		monsters[i]->x = x;
		monsters[i]->y =y;
		monsters[i]->direction = NORTH;
		monsters[i]->level = level;
		monsters[i]->hp = hp ;
		monsters[i]->owned_object = owned_object;
		monsters[i]->last_move = 0;
		monsters[i]->live=1;
	}
	return monsters ;
}
//Free
void monsters_free(struct monster** monsters,int nb_monsters ) {
	assert(monsters);
	for (int i =0 ; i<nb_monsters ; i++){
		free(monsters[i]);
	}
	free(monsters);
}
// POSITION

int monster_get_x( struct monster ** monsters ,int id){
	assert(monsters);
	assert(monsters[id]);
	struct monster * monster = monsters[id];
	return (monster->x);
}


int monster_get_y( struct monster ** monsters ,int id){
	assert(monsters);
	assert(monsters[id]);
	struct monster * monster = monsters[id];
	return (monster->y);
}

void monster_set_x( struct monster ** monsters ,int id , int x ){
	assert(monsters);
	assert(monsters[id]);
	((monsters[id])->x) = x;
}


void monster_set_y( struct monster ** monsters ,int id , int y){
	assert(monsters);
	assert(monsters[id]);
	((monsters[id])->y) = y;
}

void monster_set_position( struct monster ** monsters ,int id ,int x , int y ){
	assert(monsters);
	assert(monsters[id]);
	((monsters[id])->x) = x;
	((monsters[id])->y) = y;
}


// direction

void monster_set_current_way(struct monster ** monsters ,int id , enum direction way) {
	assert(monsters);
	assert(monsters[id]);
	((monsters[id])->direction) = way;
}

//HP
int monster_get_hp(struct monster ** monsters ,int id ) {
	assert(monsters);
	assert(monsters[id]);
	return ( (monsters[id]->hp))  ;
}

void monster_inc_hp(struct monster ** monsters ,int id ) {
	assert(monsters);
	assert(monsters[id]);
	(monsters[id]->hp) +=1;
}

void monster_dec_hp(struct monster** monsters ,int id ) {
	assert(monsters);
	assert(monsters[id]);
	if (monsters[id]->hp > 1)
		(monsters[id]->hp) -= 1 ;
	else{
		monsters[id]->live =0;
	}
}

void monster_set_hp(struct monster** monsters ,int id ,int x ) {
	assert(monsters);
	assert(monsters[id]);
	(monsters[id]->hp) = x;
}

// level
int monster_get_level( struct monster ** monsters ,int id){
	assert(monsters);
	assert(monsters[id]);
	return ((monsters[id])->level);
}

void monster_set_level( struct monster ** monsters ,int id , int x ){
	assert(monsters);
	assert(monsters[id]);
	((monsters[id])->level) = x;
}

// owned_object

int monster_get_owned_object( struct monster ** monsters ,int id){
	assert(monsters);
	assert(monsters[id]);
	return ((monsters[id])->owned_object);
}

void monster_set_owned_object( struct monster ** monsters ,int id , int x ){
	assert(monsters);
	assert(monsters[id]);
	((monsters[id])->owned_object) = x;
}


// update monsters
void monster_update(struct monster** monsters , int nb_monsters, struct player* player , struct map * map  ,int level,int difficulty){
	for (int i = 0; i<nb_monsters ; i++) {
		if (monsters[i]->live==2)
		monster_clean_monster(monsters[i], map);
		if (((monsters[i]->level) == level )&&((SDL_GetTicks() - monsters[i]->last_move)>(2000/(difficulty+1)) )&& (monsters[i]->live==1)) {
			monsters[i]->direction =monster_next_move(map , monsters[i]);
			monster_move(monsters[i], map);
			if ((monsters[i]->x )==(player_get_x(player))&& ((monsters[i]->y)==(player_get_y(player))))
				player_dec_hp(player);
			monsters[i]->last_move = SDL_GetTicks();
		}
	}
}

enum direction monster_next_move( struct map* map  ,struct monster* monster ){
	int x = monster->x;
	int y =monster->y;
	int valide = 0;
	enum direction dir;
	while (!valide){
		dir= rand()%5;
		switch (dir) {
				case NORTH:
					if(map_is_inside(map,x,y-1 )){
						if ((map_get_cell_type(map,x,y-1) == CELL_EMPTY)|| (map_get_cell_type(map ,x,y-1) == CELL_BONUS) ||((map_get_cell_type(map ,x ,y-1 ) == CELL_BOX ) && map_move_box (map , x ,y-1 ,dir))){
							valide= 1 ;
							if (map_get_cell_type(map ,x,y-1) == CELL_BONUS)
								monster->owned_object=map_get_cell_type(map ,x,y-1);
						}
					}
					break;
				case WEST:
					if(map_is_inside(map,x-1,y )){
						if ((map_get_cell_type(map,x-1,y) == CELL_EMPTY)|| (map_get_cell_type(map ,x-1,y) == CELL_BONUS) ||((map_get_cell_type(map ,x -1,y ) == CELL_BOX ) && map_move_box (map , x-1 ,y ,dir))){
							valide= 1 ;
							if (map_get_cell_type(map ,x-1,y) == CELL_BONUS)
								monster->owned_object=map_get_cell_type(map ,x-1,y);
						}
					}
					break;
			case SOUTH:
				if(map_is_inside(map,x,y+1 )){
					if ((map_get_cell_type(map,x,y+1) == CELL_EMPTY)|| (map_get_cell_type(map ,x,y+1) == CELL_BONUS) ||((map_get_cell_type(map ,x ,y+1 ) == CELL_BOX ) && map_move_box (map , x ,y+1 ,dir))){
						valide= 1 ;
						if (map_get_cell_type(map ,x,y+1) == CELL_BONUS)
							monster->owned_object=map_get_cell_type(map ,x,y+1);
					}
				}
				break;
			case EAST:
			if(map_is_inside(map,x+1,y )){
					if ((map_get_cell_type(map,x+1,y) == CELL_EMPTY)|| (map_get_cell_type(map ,x+1,y) == CELL_BONUS) ||((map_get_cell_type(map ,x +1,y ) == CELL_BOX ) && map_move_box (map , x+1 ,y ,dir))){
						valide= 1 ;
						if (map_get_cell_type(map ,x+1,y) == CELL_BONUS)
							monster->owned_object=map_get_cell_type(map ,x+1,y);
					}
				}
			break;
			default:
				valide =1;
			break;
			}

	}
	return dir ;
}

void monster_move(struct monster* monster, struct map* map) {
	switch (monster->direction) {
		case NORTH:{
				map_set_cell_type(map,monster->x,monster->y,CELL_EMPTY);
				monster->y--;
				map_set_cell_type(map,monster->x,monster->y,(CELL_MONSTER|monster->id));
				break;}
		case SOUTH:{
				map_set_cell_type(map,monster->x,monster->y,CELL_EMPTY);
				monster->y++;
				map_set_cell_type(map,monster->x,monster->y,(CELL_MONSTER|monster->id));
				break;}

		case WEST:{
				map_set_cell_type(map,monster->x,monster->y,CELL_EMPTY);
				monster->x--;
				map_set_cell_type(map,monster->x,monster->y,(CELL_MONSTER|monster->id));
				break;}

		case EAST:{
				map_set_cell_type(map,monster->x,monster->y,CELL_EMPTY);
				monster->x++;
				map_set_cell_type(map,monster->x,monster->y,(CELL_MONSTER|monster->id));
				break;}
		default:
			break ;
		}
}

void monster_clean_monster(struct monster*monster, struct map*map){
 	map_set_cell(map,monster->x,monster->y,monster->owned_object);
	monster->live=0;
}



//Display monster
void monsters_display(struct monster** monsters , int nb_monsters , int level) {
	assert(monsters);
	for ( int i =0 ; i < nb_monsters ; i++){
		struct monster* monster = monsters[i];
		if ((monster->level == level) && (monster->live) ) {
			window_display_image(sprite_get_monster(monster->direction), monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
		}
	}
}



//Save monster

void monster_save_monster(struct monster ** monsters , int nb_monsters ){
		char dest[40];
		sprintf(dest,"./data/save/monster.txt");
		FILE* fichier = fopen(dest,"w");
		fprintf( fichier , "%i \n",nb_monsters);
		for (int i = 0; i < nb_monsters; i++){
			fprintf(fichier , "%i_%i_%i_%i_%i_%i\n", monsters[i]->x , monsters[i]-> y , monsters[i]->level , monsters[i]-> hp , monsters[i]->owned_object , monsters[i]->live);
		}
		fclose(fichier);
}


//Load monster

void monster_load_monster(struct monster ** monsters , int* nb_monsters ){
		char dest[40];
		sprintf(dest,"./data/save/monster.txt");
		FILE* fichier = fopen(dest,"r");
		fscanf( fichier , "%i \n", nb_monsters);
		for (int i = 0; i < *nb_monsters; i++){
			fscanf(fichier , "%i_%i_%i_%i_%i_%i", &(monsters[i]->x) , &(monsters[i]-> y) , &(monsters[i]->level) , &(monsters[i]->hp) , &(monsters[i]->owned_object) , &(monsters[i]->live));
		}
		fclose(fichier);
}
