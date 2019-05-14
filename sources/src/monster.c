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
	int alive;
};

struct monster* monster_init(enum direction direction,int x,int y) {
	struct monster* monster = malloc(sizeof(*monster));
	if (!monster)
		error("Memory error");

	monster->direction = direction;
	monster->x = x;
	monster->y = y;
	monster->alive = 1;

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

void monster_kill(struct monster* monster) {
	assert(monster);
	monster->alive = 0;
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

enum direction monster_get_current_way(struct monster* monster) {
	assert(monster);
	return monster->direction;

}


int monster_move_aux(struct monster* monster, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;
	
	if(is_cell_monster(map,x,y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;
		break;

	case CELL_BOX:

		return 0;
		break;

	case CELL_BONUS:
		return 0;
		break;

	case CELL_KEY:

		return 0;
		break;

	case CELL_DOOR:
		return 0;
	
	case CELL_BOMB:
		return 0;
		
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

	return move;
}

void monsters_move(struct map* map){


	struct monster** monsters = map_get_monsters(map);
	int nb = map_get_nb_monsters(map);

	for(int i = 0; i < nb; i++)
	{
		if (monsters[i]->alive){
			monster_move(monsters[i],map);
		}
	}
	
}

void monster_display(struct monster* monster) {
	assert(monster);
		window_display_image(sprite_get_monster(monster->direction),
			monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
}

void monsters_display(struct map* map) {

	struct monster** monsters = map_get_monsters(map);
	int nb = map_get_nb_monsters(map);

	for(int i = 0; i < nb; i++)
	{	
		if (monsters[i]->alive) {
			
			monster_display(monsters[i]);
			
		}
		
		
	}
}

int is_monster_alive(struct monster* monster) {

assert(monster);
return(monster->alive);

}

