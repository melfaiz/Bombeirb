/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MAP_H_
#define MAP_H_

#include <player.h>

enum cell_type {
	CELL_EMPTY=0x00,   		//  0000 0000
	CELL_SCENERY=0x10, 		//  0001 0000
	CELL_BOX=0x20,   		//  0010 0000
	CELL_DOOR=0x30,      	//  0011 0000
	CELL_KEY=0x40,       	//  0100 0000
	CELL_BONUS=0x50, 		// 	0101 0000
	CELL_MONSTER=0x60, 		// 	0110 0000
	CELL_BOMB=0x70, 	   		// 	0111 0000
  CELL_EXPLOSION=0X80,    //  1000 0000
};

enum bonus_type {
	BONUS_BOMB_RANGE_DEC=1,
	BONUS_BOMB_RANGE_INC,
	BONUS_BOMB_NB_DEC,
	BONUS_BOMB_NB_INC,
	BONUS_MONSTER,
	BONUS_LIFE
};

enum scenery_type {
	SCENERY_STONE = 1,    // 0001
	SCENERY_TREE  = 2,    // 0010
	SCENERY_PRINCESS = 4  // 0100
};
enum bomb_type {
	BOMB_TT1 = 1,    //0001
	BOMB_TT2 = 2,    //0010
	BOMB_TT3 = 3,    //0011
 	BOMB_TT4 = 4,    //0100
};
enum compose_type {
	CELL_TREE     = CELL_SCENERY | SCENERY_TREE,
	CELL_STONE    = CELL_SCENERY | SCENERY_STONE,
	CELL_PRINCESS = CELL_SCENERY | SCENERY_PRINCESS,

	CELL_BOMB1 = CELL_BOMB | BOMB_TT1,
	CELL_BOMB2 = CELL_BOMB | BOMB_TT2,
	CELL_BOMB3 = CELL_BOMB | BOMB_TT3,
	CELL_BOMB4 = CELL_BOMB | BOMB_TT4,

    CELL_BOX_RANGEINC = CELL_BOX | BONUS_BOMB_RANGE_DEC,
    CELL_BOX_RANGEDEC = CELL_BOX | BONUS_BOMB_RANGE_INC,
		CELL_BOX_BOMBINC  = CELL_BOX | BONUS_BOMB_NB_INC,
    CELL_BOX_BOMBDEC  = CELL_BOX | BONUS_BOMB_NB_DEC,
    CELL_BOX_LIFE     = CELL_BOX | BONUS_LIFE,
		CELL_BOX_MONSTER     = CELL_BOX | BONUS_MONSTER,


		CELL_EXPLOSION_RANGEINC = CELL_EXPLOSION | BONUS_BOMB_RANGE_DEC,
    CELL_EXPLOSION_RANGEDEC = CELL_EXPLOSION | BONUS_BOMB_RANGE_INC,
		CELL_EXPLOSION_BOMBINC  = CELL_EXPLOSION | BONUS_BOMB_NB_DEC,
    CELL_EXPLOSION_BOMBDEC  = CELL_EXPLOSION | BONUS_BOMB_NB_INC,
    CELL_EXPLOSION_LIFE     = CELL_EXPLOSION | BONUS_LIFE,
		CELL_EXPLOSION_MONSTER     = CELL_EXPLOSION | BONUS_MONSTER,

		
		CELL_BONUS_RANGEINC = CELL_BONUS | BONUS_BOMB_RANGE_INC,
    CELL_BONUS_RANGEDEC = CELL_BONUS | BONUS_BOMB_RANGE_DEC,
		CELL_BONUS_BOMBINC  = CELL_BONUS | BONUS_BOMB_NB_INC,
    CELL_BONUS_BOMBDEC  = CELL_BONUS | BONUS_BOMB_NB_DEC,
    CELL_BONUS_LIFE     = CELL_BONUS | BONUS_LIFE,


		CELL_DOOR_ENTRY = CELL_DOOR | BOMB_TT1 ,
		CELL_DOOR_EXIT = CELL_DOOR | BOMB_TT2 ,

};

struct map;

// Create a new empty map
struct map* map_new(int width, int height);
void map_free(struct map* map);


// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);
enum cell_type map_get_cell_subtype(struct map* map, int x, int y);
// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);


// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

void display_door(struct map* map, int x, int  y);
void map_open_door(struct map* map);
int door_is_closed(struct map* map);

//void map_add_monster(struct map* map,enum direction direction,int x,int y);

// Display the map on the screen
void map_display(struct map* map);
//these functions are mine
enum bomb_type map_get_bomb_type(struct map* map, int x, int y);
unsigned char  map_cell_i_j(struct map* map, int x, int y) ;

struct map* map_get(char N);

struct monster** map_get_monsters(struct map* map);
int map_get_nb_monsters(struct map* map);
void map_set_nb_monsters(struct map* map,int nb);

void map_set_player(struct map* map,struct player* player);

void map_monster_kill(struct map* map,int i);
int is_cell_monster(struct map* map,int x,int y);

unsigned char* map_get_grid(struct map* map);

void map_bomb_effect(struct map* map, int x, int y,int range);
void map_clear_explosion(struct map* map);
void map_explosion(struct map* map, int i, int j, int range);
void map_bomb_dec(struct map* map, int range);
void map_bomb_monsters(struct map* map);
void map_bomb_player(struct map* map);

#endif /* MAP_H_ */