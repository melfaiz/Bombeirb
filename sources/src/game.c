/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <monster.h>

struct game {
	int time;
	struct map** maps;       // the game's map
	short levels;        // nb maps of the game
	short level;
	struct player* player;
	struct monster* monster;
	
};

struct game*
game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->maps = malloc(sizeof(struct game));

	game->maps[0] = map_get_static();
	game->maps[1] = map1_get_static();
	game->maps[2] = map2_get_static();

	game->levels = 3;
	game->level = 0;
	game->time=0;
	
	game->player = player_init(3);

	game->monster = monster_init();


	// Set default location of the player
	player_set_position(game->player, 1, 0);
	monster_set_position(game->monster, 6,8);


	return game;
}
int game_get_time(struct game* game){
	assert(game);
	return game->time;
}

void game_time_inc(struct game* game){

	struct monster* monster = game_get_monster(game);
	struct map* map = game_get_current_map(game);
	game->time++;
	monster_move(monster, map);
	game_empty_explosion(game);
	gamebomb(game);
	


}

void game_free(struct game* game) {
	assert(game);
	monster_free(game->monster);
	player_free(game->player);
	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->level];
}


struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}


struct monster* game_get_monster(struct game* game) {
	assert(game);
	return game->monster;
}


void game_banner_display(struct game* game) {
	assert(game);
	
	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_lives(game_get_player(game))), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(game->level), x, y);
}

void game_display(struct game* game) {
	assert(game);

	window_clear();
	game_banner_display(game);
	map_display(game_get_current_map(game));
	player_display(game->player);
	
	monster_display(game->monster);

	window_refresh();
}

static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);
	

	int i = player_get_x(game->player);
	int j = player_get_y(game->player);
	while (SDL_PollEvent(&event)) {

		switch (event.type){
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				player_move(player, map);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(player, map);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(player, map);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(player, map);
				break;
			case SDLK_SPACE:
						player_drop_bomb(player, map,i,j);
						
				break;
			default:
				break;
			}
		break;
		}
	}
	return 0;
}



void game_lvl_up(struct game* game){

	struct player* player = game_get_player(game);

	if (player_get_lvl_up(player)) {
		game->level ++;
		player_inc_lvl(player);
		monster_set_position(game->monster, 6,8);

	}
	
}

int game_update(struct game* game) {


	game_lvl_up(game);

	if (input_keyboard(game))
		return 1; // exit game

	return 0;
}

void game_empty_explosion(struct game* game){

	int w = map_get_width(game_get_current_map(game));
	int h = map_get_height(game_get_current_map(game));

	struct map* map = game_get_current_map(game);
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
	 		if (map_is_inside(map, i, j) && map_get_cell_type(map, i, j) == CELL_EXPLOSION) {
					map_set_cell_type(map, i, j, CELL_EMPTY);
				}
		}

	}
}

void game_explosion(struct map* map, int i, int j, int range) {
		
						
				map_set_cell_type(map, i, j, CELL_EXPLOSION);

	
				for(int r = 1; r < range + 1 ; r++)
				{		
						if ( map_is_inside(map, i-r, j)) {

							if (map_get_cell_type(map, i-r, j) != CELL_SCENERY   ) {
								
								map_set_cell_type(map, i-r, j, CELL_EXPLOSION);
							
							}else if(map_get_cell_type(map, i-r, j) == CELL_BOX){

								map_set_cell_type(map, i-r, j, CELL_EXPLOSION);
								break;

							}
							else{
								break;
							}
						
						}

				}


								for(int r = 1; r < range + 1 ; r++)
				{
						if ( map_is_inside(map, i, j-r) && map_get_cell_type(map, i, j-r) != CELL_SCENERY ) {
								map_set_cell_type(map, i, j-r, CELL_EXPLOSION);
						}else if(map_get_cell_type(map, i-r, j) == CELL_BOX){

								map_set_cell_type(map, i-r, j, CELL_EXPLOSION);
								break;

							}else{
							break;
						}
				}
										for(int r = 1; r < range + 1 ; r++)
				{		
						if ( map_is_inside(map, i+r, j) && map_get_cell_type(map, i+r, j) != CELL_SCENERY ) {

								map_set_cell_type(map, i+r, j, CELL_EXPLOSION);
								
						

						}else if(map_get_cell_type(map, i-r, j) == CELL_BOX){

								map_set_cell_type(map, i-r, j, CELL_EXPLOSION);
								break;

							}else{
							break;
						}
				}
				
				for(int r = 1; r < range + 1 ; r++)
				{		
						if ( map_is_inside(map, i, j+r) && map_get_cell_type(map, i, j+r) != CELL_SCENERY && map_get_cell_type(map, i, j+r) != CELL_BOX ) {
								map_set_cell_type(map, i, j+r, CELL_EXPLOSION);
								

						}else if(map_get_cell_type(map, i, j+r) == CELL_BOX){

								map_set_cell_type(map, i, j+r, CELL_EXPLOSION);
								break;

						}else{
							break;
						}
				}


}


void gamebomb(struct game* game){
	int w = map_get_width(game_get_current_map(game));
	int h = map_get_height(game_get_current_map(game));

	struct map* map = game_get_current_map(game);

	int range = player_get_range(game_get_player(game));


	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			
			if ( map_get_cell_type(map, i, j) == CELL_BOMB ) {
				if (map_get_bomb_type(map, i, j) == BOMB_TT4) {
					map_set_cell_type(map, i, j, CELL_BOMB3);
				} else if (map_get_bomb_type(map, i, j) == BOMB_TT3) {
					map_set_cell_type(map, i, j, CELL_BOMB2);
				} else if (map_get_bomb_type(map, i, j) == BOMB_TT2) {
					map_set_cell_type(map, i, j, CELL_BOMB1);
				} else if (map_get_bomb_type(map, i, j) == BOMB_TT1) {

					
					

                   game_explosion(map, i,j,range);
				
													
					
			}
				
			}
		}
	}
}