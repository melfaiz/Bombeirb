/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MONSTER_H_
#define MONSTER_H_

#include <map.h>
#include <constant.h>

struct monster;

// Creates a new monster with a given number of available bombs
struct monster* monster_init(enum direction direction,int x,int y);

void   monster_free(struct monster* monster);

// Set the position of the monster
void monster_set_position(struct monster *monster, int x, int y);

// Returns the current position of the monster
int monster_get_x(struct monster* monster);
int monster_get_y(struct monster* monster);

// Set the direction of the next move of the monster
void monster_set_current_way(struct monster * monster, enum direction direction);
enum direction monster_get_current_way(struct monster* monster);


// Move the monster according to the current direction
int monster_move(struct monster* monster, struct map* map);
void monsters_move(struct map* map);
int monster_move_aux(struct monster* monster, struct map* map, int x, int y);


// Display the monster on the screen
void monster_display(struct monster* monster);
void monsters_display(struct map* map);
void monster_kill(struct monster* monster);
int is_monster_alive(struct monster* monster);

#endif /* MONSTER_H_ */
