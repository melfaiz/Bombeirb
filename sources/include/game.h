/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <map.h>

// Abstract data type
struct game;

// Create a new game
struct game* game_new();

// Free a game
void game_free(struct game* game);

// Return the player of the current game
struct player* game_get_player(struct game* game);
struct monster* game_get_monster(struct game* game);


// Return the current map
struct map* game_get_current_map(struct game* game);

//static short game_monster_move(struct game* game);
int game_get_time(struct game* game);

void game_time_inc(struct game* game);


// Display the game on the screen
void game_display(struct game* game);
void game_empty_explosion(struct game* game);

// update
int game_update(struct game* game);
void game_lvl_up(struct game* game);

void gamebomb(struct game* game);

void game_explosion(struct map* map, int i, int j, int range);

#endif /* GAME_H_ */
