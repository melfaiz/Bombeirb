/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <map.h>

struct player {
	int x, y;
	enum direction direction;
	int bombs;
	int lives;
	int lvl_up;
	int lvl;
	int range;
};

struct player* player_init(int bombs) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->direction = NORTH;
	player->bombs = bombs;
	player->lives = 3;
	player->lvl_up = 0;
	player->lvl = 0;
	player->range = 2;
	return player;
}


void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}


void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->direction = way;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->bombs;
}

int player_get_nb_lives(struct player* player) {
	assert(player);
	return player->lives;
}

int player_get_range(struct player* player) {
	assert(player);
	return player->range;
}

void player_set_nb_lives(struct player* player,int lives) {
	assert(player);
	player->lives = lives;
}


void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->bombs += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->bombs -= 1;
}

void  player_drop_bomb(struct player* player, struct map* map,int x, int y){
	if (player_get_nb_bomb(player)>0){
				
	//	player_dec_nb_bomb(player);
		map_set_cell_type(map,x,y,CELL_BOMB4);
	}

}

void player_move_case_box(struct player* player, struct map* map, int x, int y){
	if (player->direction == NORTH  && map_is_inside(map,x,y-1) && map_get_cell_type(map,x,y-1)==CELL_EMPTY) {

			map_set_cell_type(map , x, y,CELL_EMPTY);
			map_set_cell_type(map , x, y-1,CELL_BOX);
			player->y--;
		
	}
	if (player->direction == SOUTH  && map_is_inside(map,x,y+1) && map_get_cell_type(map,x,y+1)==CELL_EMPTY ){

		map_set_cell_type(map , x, y,CELL_EMPTY);
		map_set_cell_type(map , x, y+1,CELL_BOX);
		player->y++;
		

	}
	if (player->direction == EAST  && map_is_inside(map,x+1,y) && map_get_cell_type(map,x+1,y)==CELL_EMPTY ){
		
		map_set_cell_type(map , x, y,CELL_EMPTY);
		map_set_cell_type(map , x+1, y,CELL_BOX);
		player->x++;
		
	}
	if (player->direction == WEST && map_is_inside(map,x-1,y) && map_get_cell_type(map,x-1,y)==CELL_EMPTY  ){
		
		map_set_cell_type(map , x, y,CELL_EMPTY);
		map_set_cell_type(map , x-1, y,CELL_BOX);
		player->x--;
		
	}
}

int player_move_aux(struct player* player, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;
		break;

	case CELL_BOX:
		player_move_case_box(player, map,x,y);
		return 0;
		break;

	case CELL_BONUS:
		break;

	case CELL_MONSTER:
		break;

	case CELL_KEY:
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_open_door(map);
	break;

	case CELL_DOOR:

		if ( map_get_cell_subtype(map,x,y) == 2 ) {

					if (door_is_closed(map) == 0 ) {
						player_lvl_up(player);
					}
		}		
		
		break;

	default:
		break;
	}

	//Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->direction) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1)) {
				player->y--;
				move = 1;
		}
		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1)) {
			player->y++;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y)) {
			player->x--;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y)) {
			player->x++;
			move = 1;
		}
		break;
	}

	if (move) {
		//map_set_cell_type(map, x, y, CELL_EMPTY);
	}
	return move;
}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}



void player_lvl_up(struct player*  player){
	player->lvl_up = 1;
}



int player_get_lvl_up(struct player*  player){
	return player->lvl_up;
}

int player_get_lvl(struct player*  player){
	return player->lvl;

}
void player_inc_lvl(struct player*  player){
	player->lvl ++;
	player->lvl_up = 0;

}