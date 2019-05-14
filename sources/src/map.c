/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <monster.h>
#include <player.h>

struct map {
	int width;
	int height;
	unsigned char* grid;
	int door_closed;
	struct monster** monsters;
	int nb_monsters;
	struct player* player;
};


#define CELL(i,j) ( (i) + (j) * map->width)

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->door_closed =1 ;

	

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	map->nb_monsters = 0 ;
	
  map->monsters = malloc(width*height * sizeof(struct monster*));


	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

void map_set_player(struct map* map,struct player* player){
		map->player = player;
}

		
unsigned char* map_get_grid(struct map* map){
	assert(map);
	return map->grid;
}

void map_monster_kill(struct map* map,int i){
	assert(map);
	//struct monster* monster = monster_init(SOUTH,1,1);
	monster_kill(map->monsters[i]);
//	map->monsters[i] = monster;

	}




void map_add_monster(struct map* map,enum direction direction,int x,int y){

	struct monster* monster = monster_init(direction,x,y);

	map->monsters[map->nb_monsters] = monster;

	map->nb_monsters++;

}

int map_is_inside(struct map* map, int x, int y)
{
	if ( x+1>map_get_width(map) || (x<0) || (y<0) ||y+1>map_get_height(map) ){
			return 0;
	}	else{
			 	return 1;
	}

}

void map_free(struct map* map)
{
	if (map == NULL )
		return;

	struct monster** monsters = map->monsters;

	for(int i = 0; i < map->nb_monsters; i++)
			monster_free(monsters[i]);

	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0;
}

enum cell_type map_get_cell_subtype(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0x0f;
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

//this function is mine
unsigned char  map_cell_i_j(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] ;
}

void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;

	case BONUS_LIFE:
		window_display_image(sprite_get_banner_life(), x, y) ;
	break;
	}

	
}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	case SCENERY_PRINCESS:
	   window_display_image(sprite_get_princess(), x, y);
	   break;
	}
}

void display_bomb(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case BOMB_TT1 :
		window_display_image(sprite_get_bomb1(), x, y);
		break;

	case BOMB_TT2 :
		window_display_image(sprite_get_bomb2(), x, y);
		break;
	case BOMB_TT3 :
		window_display_image(sprite_get_bomb3(), x, y);
		break;
	case BOMB_TT4 :
		window_display_image(sprite_get_bomb4(), x, y);
		break;
	}
}

enum bomb_type map_get_bomb_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0x0f;
}

void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;

	    unsigned char type = map->grid[CELL(i,j)];

			/*printf("debug type=%d\n", type);*/

	    switch (type & 0xf0) {
		case CELL_SCENERY:
		  display_scenery(map, x, y, type);
		  break;
		case CELL_MONSTER:

		map_add_monster(map,SOUTH,i,j);
		map_set_cell_type(map,i,j,CELL_EMPTY);
			break;

	    case CELL_BOX:
	      window_display_image(sprite_get_box(), x, y);
	      break;
	    case CELL_BONUS:
	      display_bonus(map, x, y, type);
	      break;
	    case CELL_KEY:
	      window_display_image(sprite_get_key(), x, y);
	      break;

	    case CELL_DOOR:

				switch ((type & 0x0f)){
	      
				
						case 1:
							window_display_image(sprite_get_door_opened(), x, y);
							break;
						case 2:
							display_door(map,x, y);
							break;

	      break;

	      }
				break;
				
			case CELL_BOMB:
	      display_bomb(map,x,y,type);
	      break;
			case CELL_EXPLOSION:
				window_display_image(sprite_get_explosion(), x, y);
				break;
	    }
	  }
	}
}



void map_open_door(struct map* map){
	if (map->door_closed)
		map->door_closed = 0;
}
int door_is_closed(struct map* map){
	if (map->door_closed)
		return 1;
	else
		return 0;
}
void display_door(struct map* map, int x, int  y){
			if (map-> door_closed)
				window_display_image(sprite_get_door_closed() ,x ,y);
			else
				window_display_image(sprite_get_door_opened() ,x ,y);

}


struct map* map_get(char N){
	//We charge the map N
	FILE* fichier = NULL;
	char mapchar[11]= "data/map_";
	mapchar[9]=N;
	mapchar[10]='\0';
	
	fichier = fopen(mapchar, "r+");

	

	int width = 0;
	int height = 0; 

	//We get the width and the height from fichier
	fscanf(fichier, "%d:%d" , &width , &height);

	

	struct map* map = map_new(width,height);

	//We browse the file and each number are put in the map->grid
	int i, j , number;
	for (i = 0; i < width; i++){
		  for (j = 0; j < height; j++){
			  fscanf(fichier, "%d" , &number);
			  map->grid[CELL(j,i)] = number;
		  }
	}

	fclose(fichier);
   	return map;
}


struct monster** map_get_monsters(struct map* map)
{
	assert(map != NULL);
	return map->monsters;
}

int map_get_nb_monsters(struct map* map)
{
	assert(map != NULL);
	return map->nb_monsters;
}

void map_set_nb_monsters(struct map* map,int nb)
{
	assert(map != NULL);
	 map->nb_monsters = nb;
}

int is_cell_monster(struct map* map,int x,int y){
	
		struct monster** monsters = map_get_monsters(map);
		for (int i = 0; i < map_get_nb_monsters(map); i++)
		{

			
			struct monster* monster = monsters[i];

			if (is_monster_alive(monster)){
				
			int x_m = monster_get_x(monster);
			int y_m = monster_get_y(monster);

			if (x==x_m && y==y_m )
			{
				return 1;
			}
			}
			
		}
		return 0;
}


void map_clear_explosion(struct map* map) {

	int w = map_get_width(map);
	int h = map_get_height(map);

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			if ( map_cell_i_j( map, i, j) == CELL_EXPLOSION_RANGEDEC ){
				map_set_cell_type(map, i, j, CELL_BONUS_RANGEDEC);
			}
			if ( map_cell_i_j( map, i, j) == CELL_EXPLOSION_LIFE ){
				map_set_cell_type(map, i, j, CELL_BONUS_LIFE);
			}
			else if(map_cell_i_j( map, i, j) == CELL_EXPLOSION_RANGEINC ){
				map_set_cell_type(map, i, j, CELL_BONUS_RANGEINC);
			}

			else if(map_cell_i_j( map, i, j) == CELL_EXPLOSION_BOMBINC ){
				map_set_cell_type(map, i, j, CELL_BONUS_BOMBINC);
			}

			else if(map_cell_i_j( map, i, j) == CELL_EXPLOSION_BOMBDEC ){
				map_set_cell_type(map, i, j, CELL_BONUS_BOMBDEC);
			}
			else if(map_cell_i_j( map, i, j) == CELL_EXPLOSION_MONSTER ){
				map_set_cell_type(map, i, j, CELL_MONSTER);
			}

			else if(map_cell_i_j( map, i, j) == CELL_EXPLOSION ){
				map_set_cell_type(map, i, j, CELL_EMPTY);
								
			}



		}
	}

}

void map_bomb_effect(struct map* map, int x, int y,int range){
	
	if ( map_is_inside(map, x, y) ) {

		switch ( map_cell_i_j( map, x, y) & 0xf0) {

		case CELL_BOX:
			
			switch ( (int)map_get_cell_subtype(map, x, y)) {
				case BONUS_LIFE:

				map_set_cell_type(map, x, y, CELL_EXPLOSION_LIFE);
				break;
				case BONUS_BOMB_NB_INC:
				map_set_cell_type(map, x, y, CELL_EXPLOSION_BOMBINC);
				break;
				case BONUS_MONSTER:
				map_set_cell_type(map, x, y, CELL_EXPLOSION_MONSTER);
				break;
				case BONUS_BOMB_NB_DEC:
					map_set_cell_type(map, x, y,  CELL_EXPLOSION_BOMBDEC);
					break;
				case BONUS_BOMB_RANGE_INC:
					map_set_cell_type(map, x, y, CELL_EXPLOSION_RANGEINC);
					break;
				case BONUS_BOMB_RANGE_DEC:
					map_set_cell_type(map, x, y, CELL_EXPLOSION_RANGEDEC);
					break;
				default:
					map_set_cell_type(map, x, y, CELL_EXPLOSION);
					break;
			}
			break;
			
		case CELL_EMPTY:
			map_set_cell_type(map, x, y, CELL_EXPLOSION);
			break;
		case CELL_SCENERY:
			break;

		case CELL_DOOR:
			break;
		case CELL_BONUS:
			map_set_cell_type(map, x, y, CELL_EXPLOSION);
			break;
		case CELL_BOMB:
			map_explosion(map, x,y,range);

			
			
			break;
		}//fin switch

	}

}


void map_explosion(struct map* map, int i, int j, int range) {
	
				player_inc_nb_bomb(map->player);
				map_set_cell_type(map, i, j, CELL_EXPLOSION);
	
				for(int r = 1; r < range + 1 ; r++)
				{		
						if ( map_is_inside(map, i-r, j)) {

							if (map_get_cell_type(map, i-r, j) != CELL_SCENERY   && map_get_cell_type(map, i-1, j) != CELL_KEY) {
								if (map_get_cell_type(map, i-r, j) == CELL_BOX ) {
									map_bomb_effect(map,i-r, j,range);
									break;
								}
								map_bomb_effect(map,i-r, j,range);
							}else{
								break;
							}
						
						}

				}

	
				for(int r = 1; r < range + 1 ; r++)
				{		
						if ( map_is_inside(map, i+r, j)) {

							if (map_get_cell_type(map, i+r, j) != CELL_SCENERY && map_get_cell_type(map, i+r, j) != CELL_KEY ) {
								if (map_get_cell_type(map, i+r, j) == CELL_BOX ) {
									map_bomb_effect(map,i+r, j,range);
									break;
								}
								map_bomb_effect(map,i+r, j,range);			
							}else{
								break;
							}
						
						}

				}

	
				for(int r = 1; r < range + 1 ; r++)
				{		
						if ( map_is_inside(map, i, j+r)) {

							if (map_get_cell_type(map, i, j+r) != CELL_SCENERY && map_get_cell_type(map, i, j+r) != CELL_KEY ) {
								if (map_get_cell_type(map, i, j+r) == CELL_BOX  ) {
									map_bomb_effect(map,i, j+r,range);
									break;
								}
								
								map_bomb_effect(map,i, j+r,range);
			
							}else{
								break;
							}
						
						}

				}

	
				for(int r = 1; r < range + 1 ; r++)
				{		
						if ( map_is_inside(map, i, j-r)) {

							if (map_get_cell_type(map, i, j-r) != CELL_SCENERY && map_get_cell_type(map, i, j-r) != CELL_KEY   ) {
								if (map_get_cell_type(map, i, j-r) == CELL_BOX )  {
									map_bomb_effect(map,i, j-r,range);
									break;
								}
								map_bomb_effect(map,i, j-r,range);
			
							}else{
								break;
							}
						
						}

				}
}
void map_bomb_dec(struct map* map, int range){

	int w = map_get_width(map);

	int h = map_get_height(map);


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

                map_explosion(map, i,j,range);
				
			}
				
			
		}
	}
}

}


void map_bomb_monsters(struct map* map){

	struct monster** monsters = map->monsters;	

		for(int i = 0; i < map_get_nb_monsters(map); i++)
		{
			if (is_monster_alive(monsters[i])) {
				int x = monster_get_x(monsters[i]);
				int y = monster_get_y(monsters[i]);
				if (map_cell_i_j(map,x,y) == CELL_EXPLOSION) {
					
					map_monster_kill(map,i);



					
				}
			}
		}

}

void map_bomb_player(struct map* map){
	struct player* player = map->player;


	int x_p = player_get_x(player);
	int y_p = player_get_y(player);

		if (map_is_inside(map,x_p,y_p) && map_cell_i_j(map,x_p,y_p) == CELL_EXPLOSION) {
			player_down(player);
		}



}