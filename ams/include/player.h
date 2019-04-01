#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>

struct player;

// Creates a new player with a given number of available bombs, hp , key and bomb_range
struct player* player_init(int bomb_number,int key_number, int hp_number, int bomb_range);

// free the player
void player_free(struct player* player);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);

// Load the player position from the map
void player_from_map(struct player* player, struct map* map);

// Move the player according to the current direction
int player_move(struct player* player, struct map* map);

// Display the player on the screen
void player_display(struct player* player);

//Check if next cell is a case, you can move after it
int move_case(struct player* player,struct map* map, int x, int y);


// Return the number of hp, player has a key or not and the current bomb range
int player_get_nb_hp(struct player* player);
int player_has_key(struct player* player);
int player_get_bomb_range(struct player* player);

//Increase & decrease bomb range and health point from player
void player_inc_bomb_range(struct player* player) ;
void player_dec_bomb_range(struct player* player);
void player_inc_nb_hp(struct player* player);
void player_dec_nb_hp(struct player* player);

//return the current direction of the player
int player_get_current_direction(struct player* player);

//return the value of player->time_attack
int player_get_time_attack(struct player* player);

//set the value of player->time_attack
void player_set_time_attack(struct player* player, int time);


// Return player from all info given
struct player* player_from_save(int x, int y,int bomb_number,int key_number, int hp_number, int bomb_range, int current_direction);


int player_get_level_game_up(struct player* player);
void player_set_pass_level_to_0(struct player* player);

// End the game when the player finds the princess
void win_the_game();

// End the game when the player has 0hp
void game_over();

#endif /* PLAYER_H_ */
