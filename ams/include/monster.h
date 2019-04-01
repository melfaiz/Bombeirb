#ifndef MONSTER_H_
#define MONSTER_H_

#include <map.h>
#include <player.h>

struct monster;

// Set and get the way the monster is looking at
void monster_set_current_way(struct monster* monster, enum direction way);
int monster_get_current_way(struct monster* monster);

//Get the coordinates of the monster
int monster_get_x(struct monster* monster);
int monster_get_y(struct monster* monster);

// Return the next monster in the list
struct monster* monster_get_next(struct monster* monster);

// Add a new monster in x,y
struct monster* set_monster_ad(struct monster* monster, struct map* map, int x, int y);

// Set the hp of the monster to 0
void set_hp0_monster(struct monster** monster, int x, int y);

// Set new timer after the pause ended so that it is as if the time had not changed
void set_new_monster_timer(struct monster* monster,int pause_time);

// Check if the monster can move or not
int monster_move_aux(struct monster* monster, struct map* map, int x, int y);

// Search a CELL_MONSTER on the map and give birth to a monster on it
struct monster* monster_from_map(struct monster* monster, struct map* map);

//Move the monster randomly
int monster_move(struct monster* monster, struct map* map, struct player* player);

// Decrease the player hp
void player_attacked(struct player* player, struct monster* monster);

// Memory allocation
void p_monster_init(struct monster** pmonster);

// Add a monster on the map. His birth is due to an explosion of a bomb on a case
void p_set_monster_ad(struct monster** monster, struct map* map, int x, int y);

//Delete monsters in the list where hp=0
struct monster* delete_monster(struct monster* monster);



//Add monsters according to the map level
//struct monster* monster_from_level(struct monster* monster, struct map* map, int N);

#endif /* MONSTER_H_ */
