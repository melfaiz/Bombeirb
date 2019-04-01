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

struct player {
	int x, y;
	enum direction current_direction;
	int nb_bombs;
	
};

struct player* player_init(int bomb_number) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->current_direction = SOUTH;
	player->nb_bombs = bomb_number;

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
	player->current_direction = way;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->nb_bombs;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bombs += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bombs -= 1;
}

static int player_move_aux(struct player* player, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;
		break;

	case CELL_BOX:
	 
		return 1;
		break;

	case CELL_BONUS:
		return 0;
		break;

	case CELL_MONSTER:
		return 0;
		break;
	case CELL_KEY:
		open_door(map);
		map_set_cell_type(map, x , y, CELL_EMPTY);
		return 1;
		break;

	default:
		return 1;
		break;
	}

	
}

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->current_direction) {
	case NORTH:

		if (player_move_aux(player, map, x, y - 1) && map_get_cell_type(map, x, y-1) == !CELL_BOX  )  {
			player->y--;
			move = 1;
		}
		if (player_move_aux(player, map, x, y - 1) && map_get_cell_type(map, x, y-1) == CELL_BOX && player_move_aux(player, map, x, y - 2) && map_get_cell_type(map, x, y-2) == CELL_EMPTY ) {
				map_set_cell_type(map,x,y-1,CELL_EMPTY);
				map_set_cell_type(map,x,y-2,CELL_BOX);
							player->y--;
			move = 1;
		}

		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1) && map_get_cell_type(map, x, y+1) == !CELL_BOX  )  {
			player->y++;
			move = 1;
		}
		if (player_move_aux(player, map, x, y + 1) && map_get_cell_type(map, x, y+1) == CELL_BOX && player_move_aux(player, map, x, y + 2) && map_get_cell_type(map, x, y+2) == CELL_EMPTY ) {
				map_set_cell_type(map,x,y+1,CELL_EMPTY);
				map_set_cell_type(map,x,y+2,CELL_BOX);
			player->y++;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x-1, y) && map_get_cell_type(map, x-1, y) == !CELL_BOX  )  {
			player->x--;
			move = 1;
		}
		if (player_move_aux(player, map, x-1, y) && map_get_cell_type(map, x-1, y) == CELL_BOX && player_move_aux(player, map, x-2, y) && map_get_cell_type(map, x-2, y) == CELL_EMPTY ) {
				map_set_cell_type(map,x-1,y,CELL_EMPTY);
				map_set_cell_type(map,x-2,y,CELL_BOX);
							player->x--;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x+1, y) && map_get_cell_type(map, x+1, y) == !CELL_BOX  )  {
			player->x++;
			move = 1;
		}
		if (player_move_aux(player, map, x+1, y) && map_get_cell_type(map, x+1, y) == CELL_BOX && player_move_aux(player, map, x+2, y ) && map_get_cell_type(map, x+2, y) == CELL_EMPTY ) {
				map_set_cell_type(map,x+1,y,CELL_EMPTY);
				map_set_cell_type(map,x+2,y,CELL_BOX);
							player->x++;
			move = 1;
		}
		break;
	}


	return move;
}


void player_drop_bomb(struct player* player, struct map* map) {

	int x = player->x;
	int y = player->y;

	if (player_get_nb_bomb(player)>0) {
			map_set_cell_type(map, x , y, CELL_BOMB);
			player_dec_nb_bomb(player);
	}

}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->current_direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}

