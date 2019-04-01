#ifndef BOMB_H_
#define BOMB_H_

#include <player.h>
#include <map.h>
#include <monster.h>



struct bomb;

//set a bomb
struct bomb* set_bomb_ad(struct player* player,struct map* map, struct bomb* game_bomb);

// Set the coordinates of the bomb
void set_bomb_position(int x,int y,struct bomb* bomb);

// Set the initial time of the bomb
void bomb_set_time(struct bomb* bomb);

// Get the initial time of the bomb
int bomb_get_init_time(struct bomb* bomb);

// Get the coordinates of the bomb
int bomb_get_x(struct bomb* bomb);
int bomb_get_y(struct bomb* bomb);

// Set a new initial timer after a pause
void set_new_bomb_timer(struct bomb* bomb,int pause_time);

// It's the main fonction for the bombs. Handle the bomb effects.
// It called every game loop.
void bomb_meca(struct bomb* bomb, struct map* map, struct player* player, struct monster** monster);

// Turn CELL_FIRE in CELL_EMPTY
void stop_fire(int bomb_range, struct map* map, int x, int y);

// Destroy all the bonus from the explosion
void destroy_bonus(int bomb_range, struct map* map, int x, int y,int block[4]);
// Destroy bonnus at x and y
void destroy_bonus_xy(struct map* map, int x, int y);

//Handle the explosion with the range and the obstacles
void set_fire(struct map* map, int x, int y, struct player* player,
				int timer, struct monster** monster, int tab[4], int birth, int bomb_range);
// Set the CELL_FIRE when bomb explodes and bring the monsters out of the case
void set_fire_xy(int birth, struct map* map, int x, int y, struct player* player, struct monster** monster);

// Return the range of the explosion that won't face obstacles
int block_fire(int bomb_range,struct map* map,int x, int y, enum direction direction);


#endif /* BOMB_H_ */
