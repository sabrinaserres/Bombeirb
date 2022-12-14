#ifndef BOSS_H_
#define BOSS_H_

#include <map.h>
#include <game.h>
#include <player.h>
#include <monster.h>
#include <boss.h>


struct boss;

struct boss * boss_init (int difficulty);
void boss_update(struct boss* boss ,struct monster**monster, struct player* player , struct map * map  ,int level,int difficulty);

void boss_free(struct boss* boss );

int boss_get_x( struct boss * boss);
int boss_get_y( struct boss * boss);
void boss_set_x( struct boss * boss , int x );
void boss_set_y( struct boss * boss , int y);
void boss_set_position( struct boss * boss ,int x , int y );

void boss_set_current_way(struct boss * boss , enum direction way);
void boss_move(struct boss* boss, struct map* map);

int boss_get_hp(struct boss * boss );
void boss_inc_hp(struct boss * boss );
void boss_dec_hp(struct boss* boss);
void boss_set_hp(struct boss* boss,int x );

enum direction boss_next_move( struct map* map  ,struct boss* boss , int x ,int y );
enum direction boss_next_move_valid( struct map* map  ,int x , int y , enum direction dir );

void display_boss(struct boss* boss);

#endif
