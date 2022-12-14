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
#include <bomb.h>
#include <game.h>
#include <monster.h>
#include <boss.h>

struct bomb {
	int timer;
	int x,y;
	int type;
	int exist;
	int direction;
	int level;
};



struct bomb** bombs_init(){ //initialisation du tableau de bombes
	struct bomb** bombs=malloc(sizeof(struct bomb*)*9);
	for(int i=0;i<9;i++){
		bombs[i] = malloc(sizeof(struct bomb));
		bombs[i]->timer = 0;
		bombs[i]->x = 0;
		bombs[i]->y = 0;
		bombs[i]->level =0;
		bombs[i]->type = 0;
		bombs[i]->exist = 0;
	}
	return bombs;
}

// permet l'initialisation de la bombe avec son type, le moment ou elle a été posée et ses coordonnées
struct bomb** bomb_init(struct bomb** bombs, struct map* map, int x, int y,int level){
	if (!bombs)
		error("Memory error");
	for(int i=0;i<9;i++){
		struct bomb* bomb = bombs[i];
		int exist = (bomb->exist);
		if(exist == 0){
			bomb->timer = SDL_GetTicks();
			bomb->x = x;
			bomb->y = y;
			bomb->type = 4;
			bomb->exist = 1;
			bomb->level = level;
			break;
		}
	}
	map_set_cell_type(map,x,y,CELL_BOMB_4);

	return bombs;
}



// POSITION DE LA BOMBES
int bomb_get_x(struct bomb* bomb){
	assert(bomb != NULL);
	return bomb->x;
}
int bomb_get_y(struct bomb* bomb){
	assert(bomb != NULL);
	return bomb->y;
}

//INITIALISATION DU TIMER POUR ACTUALISER LE TYPE DE LA BOMBE JUSQU'A L'bomb
int bomb_get_current_timer(struct bomb* bomb)
{
    assert(bomb != NULL);
    return bomb->timer;
}
void bomb_set_current_timer(struct bomb* bomb, int tmp){
    assert(bomb);
    bomb->timer = tmp;
}

// DECLARATION ET MISE A JOUR DU TYPE DE LA BOMBE PAR RAPPORT A LA LONGUEUR DE LA MECHE JUSQU'A L'bomb
int get_bomb_type(struct bomb* bomb){
    assert(bomb != NULL);
    return bomb->type;
}
void set_bomb_type(struct bomb* bomb,int type){
    assert(bomb != NULL);
    bomb->type = type;
}
void dec_bomb_type(struct bomb* bomb,int type){
    assert( bomb != NULL);
    bomb->type -=1;
}

void bombs_update(struct bomb** bombs,struct map** maps,struct monster** monsters,struct player* player,struct boss * boss , int range, int level){
	int type = 4;
	int time = SDL_GetTicks();

	for(int i=0;i<9;i++){
		struct bomb* bomb = bombs[i];

		if((bombs[i]->exist )!= 0){

			int bomb_time = bomb_get_current_timer(bomb);
			if((time - bomb_time)>1000){

				switch(bomb->type){
					case 4 :
						dec_bomb_type(bomb,type);

						map_set_cell(maps[bomb->level],bomb_get_x(bomb),bomb_get_y(bomb),CELL_BOMB_4);
						bomb->timer = SDL_GetTicks();
						break;
					case 3 :
						dec_bomb_type(bomb,type);

						map_set_cell(maps[bomb->level],bomb_get_x(bomb),bomb_get_y(bomb),CELL_BOMB_3);
						bomb->timer = SDL_GetTicks();
						break;
					case 2 :
						dec_bomb_type(bomb,type);

						map_set_cell(maps[bomb->level],bomb_get_x(bomb),bomb_get_y(bomb),CELL_BOMB_2);
						bomb->timer = SDL_GetTicks();
						break;
					case 1 :
						dec_bomb_type(bomb,type);

						map_set_cell(maps[bomb->level],bomb_get_x(bomb),bomb_get_y(bomb),CELL_BOMB_1);
						bomb->timer = SDL_GetTicks();
						break;
					case 0 :
						dec_bomb_type(bomb,type);
						bomb->exist = 2;
						bomb->timer = SDL_GetTicks();
						map_set_cell(maps[bomb->level],bomb_get_x(bomb),bomb_get_y(bomb),CELL_EMPTY);
						break;
					case -1:
					  bomb_casualties(maps,player,monsters,bombs, boss, range,level);
						player_inc_nb_bomb(player);
						bomb->exist=0;
						break;
				}
			}
		}
	}
}





int explosion_valide(struct map* map, int x, int y){ // permet de savoir si le type de la case peut etre explosée ou pas
	if (!map_is_inside(map, x, y))
		return 0;
	switch(map_get_cell_type(map,x,y) ){
		case CELL_SCENERY:
			return 0;
			break;
		case CELL_BOX:
			return 1;
			break;
		case CELL_BONUS:
			return 1;
			break;
		case CELL_BOMB :
			return 0;
			break;
		case CELL_MONSTER:
			return 0;
			break;
		case CELL_EMPTY:
			return 1;
			break;
		default:
			return 0;
			break;
	}
}

// permet d'afficher l'explosion si c'est une case ou l'explosion est 'casualties' == possible

void display_explosion(struct bomb** bombs, struct map* map,int range,int level){
	for(int i=0; i < 9 ; i++){
		struct bomb* bomb = bombs[i];
		if ((bomb->exist == 2)&&(bomb->level==level)){
			window_display_image(sprite_get_bomb(0),(bomb->x)*40,((bomb->y)*40));
			for (int j=1; j<range +1 ;j++){
				int valide = explosion_valide(map,((bomb->x)+j),bomb->y);
				if (valide == 1){
					window_display_image(sprite_get_bomb(0),((bomb->x)+j)*40,(bomb->y)*40);
				}
				else break;
			}
			for (int j=1; j < range + 1;j++){
				int valide = explosion_valide(map,((bomb->x)-j),bomb->y);
				if (valide == 1){
					window_display_image(sprite_get_bomb(0),((bomb->x)-j)*40,(bomb->y)*40);
				}
				else break;
			}
			for (int j=1; j<range + 1 ;j++){
				int valide = explosion_valide(map,bomb->x,bomb->y+j);
				if (valide == 1){
					window_display_image(sprite_get_bomb(0),(bomb->x)*40,((bomb->y)+j)*40);
				}
				else break;
			}
			for (int j=1; j<range +1;j++){
				int valide = explosion_valide(map,bomb->x,bomb->y-j);
				if (valide == 1){
					window_display_image(sprite_get_bomb(0),(bomb->x)*40,((bomb->y)-j)*40);
				}
				else break;
			}
		}
	}
}

// permet de savoir comment agir sur une case en fpnction de son type apres l'explosion

int bomb_casualties_aux(struct map* map, struct monster** monsters, struct boss * boss, int x, int y){
	if (!map_is_inside(map, x, y))
		return 0;
	switch(map_get_cell_type(map,x,y) ){
		case CELL_SCENERY:
			return 0;
			break;
		case CELL_BOX:
			map_set_cell(map,x,y,(CELL_BONUS|(map_get_cell(map,x,y)&0x0f)));
			return 1;
			break;
		case CELL_BONUS:
			map_set_cell(map,x,y,CELL_EMPTY);
			return 1;
			break;
		case CELL_BOMB :
			return 0;
			break;
		case CELL_MONSTER:
			if ((map_get_cell(map,x,y)&0x0f)<0x0f)
				monster_dec_hp(monsters,(map_get_cell(map,x,y)&0x0f));
			else
				boss_dec_hp(boss);
			return 1;
			break;
		case CELL_EMPTY:
			return 1;
			break;
		default:
			return 0;
			break;
	}
}


// permet de savoir comment agir sur une case du nord/sud/est/ouest en fonction de la range apres l'explosin en fesant appel a bomb_casualties

void bomb_casualties(struct map** maps,struct player* player, struct monster** monsters, struct bomb** bombs, struct boss * boss ,int range, int level){
	for(int i=0; i < 9 ; i++){
		int casualties = 1;
		struct bomb* bomb = bombs[i];
		if ((bomb->exist == 2)&&(bomb->level==level)){
			for (int j=1; j<range +1 ;j++){
				casualties = bomb_casualties_aux(maps[bomb->level],monsters,boss,((bomb->x)+j),bomb->y);
				if (!casualties)
					break;
			}
			for (int j=1; j < range + 1;j++){
				casualties = bomb_casualties_aux(maps[bomb->level],monsters,boss,((bomb->x)-j),bomb->y);
				if (!casualties)
			   	break;
			}
			for (int j=1; j<range + 1 ;j++){
				casualties = bomb_casualties_aux(maps[bomb->level],monsters,boss,bomb->x,bomb->y+j);
				if (!casualties)
					break;

			}
			for (int j=1; j<range +1;j++){
				casualties = bomb_casualties_aux(maps[bomb->level],monsters,boss,bomb->x,bomb->y-j);
				if (!casualties)
					break;
			}
		}
	}
}



// save bombs
void bomb_save_bomb(struct bomb** bombs){
	char dest[40];
	sprintf(dest,"./data/save/bombs.txt");
	FILE* fichier = fopen(dest,"w");
	for (int i = 0; i < 9; i++){
		fprintf(fichier , "%i_%i_%i_%i_%i_\n", bombs[i]->x , bombs[i]-> y , bombs[i]->level , bombs[i]-> type , bombs[i]->exist);
	}
	fclose(fichier);
}
//load bombs
void bomb_load_bomb(struct bomb** bombs){
	char dest[40];
	sprintf(dest,"./data/save/bombs.txt");
	FILE* fichier = fopen(dest,"r");
	for (int i = 0; i < 9; i++){
		fscanf(fichier , "%i_%i_%i_%i_%i_\n", &(bombs[i]->x) , &(bombs[i]-> y) , &(bombs[i]->level) , &(bombs[i]-> type) , &(bombs[i]->exist));
		bombs[i]->timer=SDL_GetTicks();
	}
	fclose(fichier);
}
