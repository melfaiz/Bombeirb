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

struct monster {
	int x, y;
	enum direction direction;
};

struct monster* monster_init() {
	struct monster* monster = malloc(sizeof(*monster));
	if (!monster)
		error("Memory error");

	monster->direction = SOUTH;

	return monster;
}


void monster_set_position(struct monster *monster, int x, int y) {
	assert(monster);
	monster->x = x;
	monster->y = y;
}


void monster_free(struct monster* monster) {
	assert(monster);
	free(monster);
}

int monster_get_x(struct monster* monster) {
	assert(monster != NULL);
	return monster->x;
}

int monster_get_y(struct monster* monster) {
	assert(monster != NULL);
	return monster->y;
}

void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->direction = way;
}

void monster_change_direction(struct monster* monster){
	if (monster->direction == NORTH)
		{monster->direction=SOUTH;}
	if (monster->direction== SOUTH)
		{monster->direction= NORTH;}
	if (monster->direction== EAST)
		{monster->direction= WEST;}
	if (monster->direction== WEST)
		{monster->direction= EAST;}
	
}


int monster_move_aux(struct monster* monster, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;
	

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:

	//		monster_change_direction(monster);
			
		return 0;
		break;

	case CELL_BOX:
	//	monster_change_direction(monster);
		return 0;
		break;

	case CELL_BONUS:
	//	monster_change_direction(monster);
		break;

	case CELL_KEY:
	//	monster_change_direction(monster);
		return 0;

	case CELL_DOOR:
	//	monster_change_direction(monster);
		break;
	default:
		break;
	}

	//monster has moved
	return 1;
}

int monster_move(struct monster* monster, struct map* map) {
	int x = monster->x;
	int y = monster->y;
	int move = 0;

	switch (monster->direction) {
	case NORTH:
		if (monster_move_aux(monster, map, x, y - 1)) {
				monster->y--;
				move = 1;
		}else
		{
			monster->direction = rand()%4 ;
		}
		
		break;

	case SOUTH:
		if (monster_move_aux(monster, map, x, y + 1)) {
			monster->y++;
			move = 1;
		}else
		{
			monster->direction = rand()%4 ;
		}
		break;

	case WEST:
		if (monster_move_aux(monster, map, x - 1, y)) {
			monster->x--;
			move = 1;
		}else
		{
			monster->direction = rand()%4 ;
		}
		break;

	case EAST:
		if (monster_move_aux(monster, map, x + 1, y)) {
			monster->x++;
			move = 1;
		}else
		{
			monster->direction = rand()%4 ;
		}
		break;
	}

	if (move) {
		//map_set_cell_type(map, x, y, CELL_EMPTY);
	}
	return move;
}

void monster_display(struct monster* monster) {
	assert(monster);
	window_display_image(sprite_get_monster(monster->direction),
			monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
}
