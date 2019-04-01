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

// Return the monsters of the current game
struct monster* game_get_monster(struct game* game);

// Return the current map
struct map* game_get_map(struct game* game);

// Display the game on the screen
void game_display(struct game* game);

// Handle the moves of the monsters and the bombs in the game
void game_meca(struct game* game); //new

// update
int game_update(struct game* game);

//pause the game
int pause_game();

//save the game
void save_game(struct game* game);

// Help to free the monsters and bombs when loading the next level
void game_next(struct game* game);

//load the game
void load_game();

#endif /* GAME_H_ */
