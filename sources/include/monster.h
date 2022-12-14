#ifndef MONSTER_H_
#define MONSTER_H_

#include <player.h>

struct monster;

// init monsters
struct monster** monsters_init();

// Free monsters
void monsters_free(struct monster** monsters,int nb_monsters );

//set or get a monster position

int monster_get_x( struct monster ** monsters ,int id);
int monster_get_y( struct monster ** monsters ,int id);
void monster_set_x( struct monster ** monsters ,int id , int x);
void monster_set_y( struct monster ** monsters ,int id , int y);
void monster_set_position( struct monster ** monsters ,int id , int x ,  int y);
//set a monster direction
void monster_set_current_way(struct monster ** monsters ,int id , enum direction way);
// set or get  monster hp
int monster_get_hp(struct monster ** monsters ,int id);
void monster_inc_hp(struct monster ** monsters ,int id );
void monster_dec_hp(struct monster ** monsters ,int id );
void monster_set_hp(struct monster ** monsters ,int id ,int x );

// set or get level
int monster_get_level( struct monster ** monsters ,int id);
void monster_set_level( struct monster ** monsters ,int id , int x );

// set or get owned object
int monster_get_owned_object( struct monster ** monsters ,int id);
void monster_set_owned_object( struct monster ** monsters ,int id , int x );

//display monsters
void monsters_display(struct monster** monsters , int nb_monsters , int level);

// update monsters
void monster_update(struct monster** monsters , int nb_monsters,struct player* player, struct map * map  ,int level , int difficulty);
//used when monster die
void monster_clean_monster(struct monster *monster, struct map* map);
// move monster
enum direction monster_next_move( struct map* map ,struct monster* monster);
void monster_move(struct monster* monster, struct map* map);

//Save Load monsters
void monster_save_monster(struct monster ** monsters , int nb_monsters );
void monster_load_monster(struct monster ** monsters , int* nb_monsters );
#endif
