/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>

struct player;

// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number);
void   player_free(struct player* player);

void set_player(struct player *player, int x, int y,int level,enum direction direction,int nb_bombs,int lives,int range);

// Set the position of the player
void player_set_position(struct player *player, int x, int y);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);
void player_drop_bomb(struct player* player, struct map* map , int i, int j);
void player_move_case_box(struct player* player, struct map* map, int x, int y);
int player_move_case_Door(struct player* player, struct map* map, int x, int y);


// Move the player according to the current direction
int player_move(struct player* player, struct map* map);

// Display the player on the screen
void player_display(struct player* player);
int player_get_nb_lives(struct player* player);
void player_set_nb_lives(struct player* player,int lives);

int player_get_lvl(struct player*  player);
void player_set_lvl(struct player*  player,int level);

void player_inc_lvl(struct player*  player);
int player_get_range(struct player* player);

void player_set_nb_range(struct player* player,int range);

enum direction player_get_current_way(struct player* player);

void player_dec_lvl(struct player*  player);

void player_down(struct player*  player);

int end_game(struct player* player);
 
#endif /* PLAYER_H_ */
