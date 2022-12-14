#ifndef BOMB_H_
#define BOMB_H_

#include <map.h>
#include <player.h>
#include <monster.h>
#include <boss.h>


struct bomb;


struct bomb** bombs_init();

struct bomb** bomb_init( struct bomb **bombs, struct map* map,int x, int y, int level);

int bomb_get_x(struct bomb* bomb);
int bomb_get_y ( struct bomb* bomb);

int bomb_get_current_timer( struct bomb* bomb);
void bomb_set_current_timer(struct bomb* bomb, int timer);

int get_bomb_type(struct bomb* bomb);
void set_bomb_type(struct bomb* bomb, int type);
void dec_bomb_type(struct bomb* bomb, int type);
void bombs_update(struct bomb** bombs,struct map** maps, struct monster** monsters, struct player* player,struct boss * boss , int range,int level);
int explosion_valide(struct map* map, int x, int y);
void display_explosion(struct bomb** bombs, struct map* map,int range, int level);

int bomb_casualties_aux(struct map* map, struct monster** monsters, struct boss * boss ,int x, int y);
void bomb_casualties(struct map** maps ,struct player* player, struct monster** monsters, struct bomb** bombs, struct boss * boss , int range, int level);

// Save load bombs
void bomb_save_bomb(struct bomb** bombs);
void bomb_load_bomb(struct bomb** bombs);


#endif /* BOMB_H_ */
