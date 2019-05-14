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
	int lvl;
	int range;
	int time_hurt;
	int end;
};

struct player* player_init(int bombs) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->direction = NORTH;
	player->bombs = bombs;
	player->lives = 3;
	player->lvl = 0;
	player->range = 2;
	player->end = 0;
	player->time_hurt=-10000; 
	return player;
}


void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}

void set_player(struct player *player, int x, int y,int level,enum direction direction,int nb_bombs,int lives,int range) {
	assert(player);

	player->x = x;
	player->y = y;
	player->lvl = level;
	player->direction = direction;
	player->bombs = nb_bombs;
	player->lives = lives;
	player->range = range;
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

enum direction player_get_current_way(struct player* player) {
	assert(player);
	return player->direction;
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

void player_set_nb_range(struct player* player,int range) {
	assert(player);
	if (range > 0)
	{
		player->range = range;
	}
	
	
}


void player_inc_nb_bomb(struct player* player) {
	assert(player);
	if (player->bombs < 8) {
		player->bombs += 1;
	}
	
	
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);

	if ( player->bombs > 0)
	{
	player->bombs -= 1;
	}

}

void  player_drop_bomb(struct player* player, struct map* map,int x, int y){


	if (player_get_nb_bomb(player)>0 && map_get_cell_type(map,x,y) != CELL_DOOR ){
		player_dec_nb_bomb(player);
		map_set_cell_type(map,x,y,CELL_BOMB4);
	}

	

}

void player_move_case_box(struct player* player, struct map* map, int x, int y){



	if (player->direction == NORTH  && map_is_inside(map,x,y-1) && map_get_cell_type(map,x,y-1)==CELL_EMPTY && !is_cell_monster(map,x,y-1)) {

			
			map_set_cell_type(map , x, y-1, CELL_BOX | map_get_cell_subtype(map,x,y) ) ;
			map_set_cell_type(map , x, y,CELL_EMPTY);
			player->y--;
		
	}
	if (player->direction == SOUTH  && map_is_inside(map,x,y+1) && map_get_cell_type(map,x,y+1)==CELL_EMPTY && !is_cell_monster(map,x,y+1)){

		
		map_set_cell_type(map , x, y+1,CELL_BOX | map_get_cell_subtype(map,x,y) );
		map_set_cell_type(map , x, y,CELL_EMPTY);
		player->y++;
		

	}
	if (player->direction == EAST  && map_is_inside(map,x+1,y) && map_get_cell_type(map,x+1,y)==CELL_EMPTY && !is_cell_monster(map,x+1,y)){
		
	
		map_set_cell_type(map , x+1, y,CELL_BOX | map_get_cell_subtype(map,x,y)) ;
		map_set_cell_type(map , x, y,CELL_EMPTY);
		player->x++;
		
	}
	if (player->direction == WEST && map_is_inside(map,x-1,y) && map_get_cell_type(map,x-1,y)==CELL_EMPTY && !is_cell_monster(map,x-1,y) ){
		
		
		map_set_cell_type(map , x-1, y,CELL_BOX | map_get_cell_subtype(map,x,y) );
		map_set_cell_type(map , x, y,CELL_EMPTY);
		player->x--;
		
	}
}

int player_move_aux(struct player* player, struct map* map, int x, int y) {
	int nb_life;
	int nb_range;
	int nb_bombs;


	
	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		switch ( (int)map_get_cell_subtype(map, x, y)){
			case SCENERY_PRINCESS:
				window_display_image(sprite_get_win(), 0,0);

				window_refresh();

				SDL_Delay(3000);
				player->end = 1;
			break;
		}
		return 0;
		break;

	case CELL_BOX:
		player_move_case_box(player, map,x,y);
		return 0;
		break;

	case CELL_BONUS:

		switch ( (int)map_get_cell_subtype(map, x, y)) {

			case BONUS_LIFE:

				nb_life=player_get_nb_lives(player)+1;

				if (nb_life<8)
				{
					player_set_nb_lives( player, nb_life);
				}
				
				
				
				break;

			case BONUS_BOMB_NB_INC:
				player_inc_nb_bomb( player);
				
				break;

			case BONUS_BOMB_NB_DEC:

				nb_bombs = player_get_nb_bomb(player);

				if (nb_bombs>1)
				 {
				player_dec_nb_bomb( player);
				 }
				break;
				

			case BONUS_BOMB_RANGE_INC:
			 	 nb_range=player_get_range( player)+1;
				player_set_nb_range( player, nb_range);
				break;
				

			case BONUS_BOMB_RANGE_DEC:
				  nb_range=player_get_range(player)-1;
				  if (nb_range>1)
				  {
					  player_set_nb_range( player, nb_range);
				  }
				  
		    	
				break;
				
			default:
				break;
				
	  }
		map_set_cell_type(map, x, y, CELL_EMPTY);
		


	case CELL_MONSTER:
		break;		
	case CELL_KEY:
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_open_door(map);
	break;

	case CELL_DOOR:

		if ( map_get_cell_subtype(map,x,y) == 2 ) {

					if (door_is_closed(map) == 0 ) {
						player_inc_lvl(player);
							
					}
		}else if(map_get_cell_subtype(map,x,y) == 1)
		{
			player_dec_lvl(player);
		
		}
			
		
		break;

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

	if (move  && (map_get_cell_type(map, x, y)== CELL_BONUS ) ) {
		//map_set_cell_type(map, x, y, CELL_EMPTY);
	}
	return move;
}

void player_display(struct player* player) {
	assert(player);

		int current_time = SDL_GetTicks();
		if ((current_time - player->time_hurt) < 2000){
			window_display_image(sprite_get_player(4),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
		}else{

		
		window_display_image(sprite_get_player(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
		}
}


int player_get_lvl(struct player*  player){
	return player->lvl;

}

void player_set_lvl(struct player*  player,int level){

	player->lvl = level;

}



void player_inc_lvl(struct player*  player){
	player->lvl ++;
}
void player_dec_lvl(struct player*  player){
	player->lvl --;
}


void player_down(struct player*  player){

	if (player->lives>0 ) {
			
		int current_time = SDL_GetTicks();
		if ((current_time - player->time_hurt) > 2000){
			player->lives -- ;
			player->time_hurt = SDL_GetTicks();
		}
		
	}else{
				window_display_image(sprite_get_over(), 0,0);

				window_refresh();

				SDL_Delay(3000);
				player->end = 1;
	
	}	

}


int end_game(struct player* player){
	return player->end ;
}