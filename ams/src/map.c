#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <bomb.h>

struct map {
	int width;
	int height;
	unsigned char* grid;
	int map_level;
};

#define CELL(i,j) ( i + j * map->width)


struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;
	map->map_level = 1;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

int map_is_inside(struct map* map, int x, int y)
{
	assert(map);
	return 1;

}

void map_free(struct map* map)
{
	if (map == NULL )
		return;
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

int map_get_grid(struct map* map, int i, int j){
	assert(map);
	return map->grid[CELL(i,j)];
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0x0f;
}

enum cell_type map_get_sub_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0;
}


void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type & 0x0f ;
}


void  map_set_type(struct map* map, int x, int y, enum bomb_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

int map_get_level(struct map* map){
	assert(map);
	return map->map_level;
}

void map_change_level(struct map* map,int maplvl) {
	assert(map);
	map->map_level=maplvl +1;
}

unsigned char* map_get_full_grid(struct map* map){
	assert(map);
	return map->grid;
}


struct map* map_set_maplvl(struct map* map, int map_level){
	map->map_level = map_level;
	return map;
}

void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type >> 4) {
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
		window_display_image(sprite_get_bonus(BONUS_LIFE), x, y);
		break;
	}
}
void display_bomb(struct map* map, int x, int y, unsigned char type){
	switch (type >> 4) {
	case WICK1:
		window_display_image(sprite_get_bomb(WICK1), x, y);
		break;
	case WICK2:
		window_display_image(sprite_get_bomb(WICK2), x, y);
		break;
	case WICK3:
		window_display_image(sprite_get_bomb(WICK3), x, y);
		break;
	case WICK4:
		window_display_image(sprite_get_bomb(WICK4), x, y);
		break;
	}

}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type >> 4) { // sub-types are encoded with the 4 most significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	}
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



	    switch (type & 0x0f) {
		case CELL_SCENERY:
		  display_scenery(map, x, y, type);
		  break;
	    case CELL_CASE:
	      window_display_image(sprite_get_box(), x, y);
	      break;
	    case CELL_BONUS:
	      display_bonus(map, x, y, type);
	      break;
	    case CELL_KEY:
	      window_display_image(sprite_get_key(), x, y);
	      break;
	    case CELL_BOMB:
		  display_bomb( map, x, y, type);
		  break;
	    case CELL_FIRE:
	      window_display_image(sprite_get_fire(), x, y);
	      break;
	    case CELL_PRINCESS:
	      window_display_image(sprite_get_princess(), x, y);
	      break;
	    case CELL_DOOR:
	      switch ((type & 0x80) >> 7){
	      // Here we have to handle the case when the door is opened or closed
	      case 1:
	        window_display_image(sprite_get_door_opened(), x, y);
	        break;
	      case 0:
	        window_display_image(sprite_get_door_closed(), x, y);
	        break;
	      break;
	      }
	    }
	  }
	}
}

struct map* map_get_default(void)
{
	struct map* map = map_new(MAP_WIDTH, MAP_HEIGHT);

	unsigned char themap[MAP_WIDTH * MAP_HEIGHT] = {
	  CELL_PLAYER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_CASE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_CASE, CELL_BOMB, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_CASE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_CASE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_BONUS, CELL_EMPTY, CELL_CASE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_CASE, CELL_TREE, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_KEY, CELL_EMPTY,  CELL_EMPTY,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_EMPTY,
	  CELL_CASE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY,  CELL_CASE_LIFE, CELL_EMPTY,
	  CELL_EMPTY, CELL_CASE_MONSTER, CELL_DOOR, CELL_EMPTY, CELL_SCENERY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
		};

	for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
		map->grid[i] = themap[i];

	return map;
}

struct map* map_get(char N){
	//We charge the map N
	FILE* fichier = NULL;
	char mapname[11]= "data/map_0";
	mapname[9]=N;
	mapname[10]='\0';
	fichier = fopen(mapname, "r+");
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

int door_open(int x, int y, struct map* map){
	unsigned char type = map->grid[CELL(x,y)];
	return ((type & 0x80) >> 7);
}

char next_level(struct map* map){
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	for (int i = 0; i < map->width; i++) {
		  for (int j = 0; j < map->height; j++) {
				  char type = map->grid[CELL(i,j)];
				  if ((type & 0x0f) == CELL_DOOR){
					  return ((type & 0x70) >> 4);
				  }//if
		  }//for
	}//for

	return -1;
}
