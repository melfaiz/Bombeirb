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
struct monster** game_get_monsters(struct game* game);


// Return the current map
struct map* game_get_current_map(struct game* game);
int game_get_current_lvl(struct game* game);

//static short game_monster_move(struct game* game);
int game_get_time(struct game* game);

void game_speed_one(struct game* game);
void game_speed_lvl(struct game* game);

void game_monsters_move(struct game* game);



// update
int game_update(struct game* game);
void game_display(struct game* game);


void bomb_collision(struct game* game);
void monster_collision(struct game* game);

int pause_game();
int menu_game();


void save_game(struct game* game);
void load_game(struct game* game);

void game_monster_get_player(struct game* game);

#endif /* GAME_H_ */
