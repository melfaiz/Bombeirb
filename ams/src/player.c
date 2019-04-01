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
	enum direction current_direction;
	int nb_bomb;
	int key;
	int nb_hp;
	int bomb_range;
	int time_attack; // Needed to not suffer too much damage from monsters
	int level_game_up; // Boolean to say if the player pass to an other level
};


struct player* player_init(int bomb_number,int key_number, int hp_number, int bomb_range) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->current_direction = SOUTH;
	player->nb_bomb = bomb_number;
	player->key = key_number;
	player->nb_hp = hp_number;
	player->bomb_range = bomb_range;
	player->time_attack=-5000; //Set to have the right value at the start, a monster can decrease his life at the beginning
	player->level_game_up = 0; // If = 1 change the level
	return player;
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

int player_get_nb_hp(struct player* player) {
	assert(player);
	return player->nb_hp;
}

void player_inc_nb_hp(struct player* player) {
	assert(player);
	if (player->nb_hp < 8){
		player->nb_hp += 1;
	}
}

void player_dec_nb_hp(struct player* player) {
	assert(player);
	if (player->nb_hp > 0){
		player->nb_hp -= 1;
	}
	if (player->nb_hp == 0){
		int white_bloc = ((12 * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
		int x = white_bloc + SIZE_BLOC;
		int y = 12.1 * SIZE_BLOC;
		window_display_image(sprite_get_number(0) ,x, y);
		game_over();
	}
}

int player_has_key(struct player* player) {
	assert(player);
	return player->key;
}

int player_get_bomb_range(struct player* player) {
	assert(player);
	return player->bomb_range;
}

void player_inc_bomb_range(struct player* player) {
	assert(player);
	player->bomb_range += 1 ;
}

void player_dec_bomb_range(struct player* player) {
	assert(player);
	if (player->bomb_range >0){
		player->bomb_range -= 1 ;
	}
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->nb_bomb;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	if (player->nb_bomb <8){
		player->nb_bomb += 1;
	}
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	if (player->nb_bomb >0){
		player->nb_bomb -= 1;
	}
}	

int player_get_current_direction(struct player* player){
	assert(player);
	return player->current_direction;
}
int player_get_time_attack(struct player* player){
	assert(player);
	return player->time_attack;
}

void player_set_time_attack(struct player* player,int time){
	player->time_attack = time;
}

int player_get_level_game_up(struct player* player){
	assert(player);
	return player->level_game_up;
}

void player_set_pass_level_to_0(struct player* player){
	assert(player);
	player->level_game_up = 0;
}
void player_from_map(struct player* player, struct map* map) {
	assert(player);
	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
	  for (j = 0; j < map_get_height(map); j++) {	
	    if (map_get_cell_type(map, i, j) == CELL_PLAYER) {
	      player->x = i;
	      player->y = j;
	    }
	  }
	}
}



static int player_move_aux(struct player* player, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;
	int current_time;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;
		break;

	case CELL_CASE:
		return 1;
			break;

	case CELL_BONUS://3.4
		switch (map_get_sub_type(map,x,y) >> 4){
		case BONUS_LIFE:
			player_inc_nb_hp(player);
			return 1;
			break;
		case BONUS_BOMB_RANGE_DEC:
			if (player->bomb_range >1)
				player_dec_bomb_range(player);
			return 1;
			break;
		case BONUS_BOMB_RANGE_INC:
			player_inc_bomb_range(player);
			return 1;
			break;
		case BONUS_BOMB_NB_DEC:
			player_dec_nb_bomb(player);
			return 1;
			break;
		case BONUS_BOMB_NB_INC:
			player_inc_nb_bomb(player);
			return 1;
			break;
		default:
			break;
		}
		break;

	case CELL_MONSTER:
		//Can't be hurt for  5000time
			current_time = SDL_GetTicks();
		if ((current_time - player->time_attack) > 5000){
			player_dec_nb_hp(player);
			player->time_attack = SDL_GetTicks();
		}
		return 1;
		break;

	case CELL_PLAYER:
		break;
	case CELL_DOOR:

		//If the door is opened or if the player have a key
		if (door_open(x,y,map) == 1 || (player->key == 1) ){
			char m= '0';
			int stocklvl=map_get_level(map);
			m=(next_level(map)+48);//get the next level from the door on the map
			map_free(map);
			map = map_get(m);//Load the new map in the data
			map_change_level(map,stocklvl);//Set the new map_level
			player->key = 0;
			player->level_game_up = 1;
			return 1;
		}
		else{
			return 0;
		}
		break;
	case CELL_KEY:
		player->key = 1;
		return 1;
		break;

	case CELL_FIRE: // Handle the decrease of hp in the function set_fire_xy
		current_time = SDL_GetTicks();
		if ((current_time - player_get_time_attack(player)) > 2000){
			player_dec_nb_hp(player);
			player_set_time_attack(player,SDL_GetTicks());
		}
		return 1;
		break;
	case CELL_PRINCESS:
		win_the_game();
		return 1;
		break;
			
	default:
		break;
	}


	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map) { // The CELL_CASE are handled here with the player move
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->current_direction) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1) && y > 0) {
			if (map_get_cell_type(map,x,y-1) == CELL_CASE){
				if (move_case(player,map,x,y)){
					player->y--;
					map_set_type(map, x,y-2, CELL_CASE | map_get_sub_type(map, x, y-1));//we move the bonnus also
					move = 1;
					break;
				}
				else
					break;
			}
			player->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1) && y < MAP_HEIGHT-1) {
			if (map_get_cell_type(map,x,y+1) == CELL_CASE){
				if (move_case(player,map,x,y)){
					player->y++;
					map_set_type(map, x,y+2, CELL_CASE | map_get_sub_type(map, x, y+1));
					move = 1;
					break;
				}
				else
					break;
			}
			player->y++;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y) && x > 0) {
			if (map_get_cell_type(map,x-1,y) == CELL_CASE){
				if (move_case(player,map,x,y)){

					player->x--;
					map_set_type(map, x-2,y, CELL_CASE | map_get_sub_type(map, x-1, y));

					move = 1;
					break;
				}
				else
					break;

			}
			player->x--;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y) && x < MAP_WIDTH-1) {
			if (map_get_cell_type(map,x+1,y) == CELL_CASE){
				if (move_case(player,map,x,y)){
					player->x++;
					map_set_type(map, x+2,y, CELL_CASE | map_get_sub_type(map, x+1, y));
					move = 1;
					break;
				}
				else
					break;
			}
			player->x++;
			move = 1;
		}
		break;

	}

	if (move) {
		if (map_get_cell_type(map,x,y) != CELL_BOMB){//We don't want to erase the CELL_BOMB
			map_set_cell_type(map, x, y, CELL_EMPTY);
		}
		map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
	}
	return move;

}



int move_case(struct player* player,struct map* map, int x, int y){
	switch (player->current_direction) {
		case NORTH:
			if (map_get_cell_type(map,x,y-1) == CELL_CASE &&
					map_get_cell_type(map,x,y-2) == CELL_EMPTY && y-2 >= 0)
				return 1;
			else
				return 0;

		case SOUTH:
			if (map_get_cell_type(map,x,y+1) == CELL_CASE &&
					map_get_cell_type(map,x,y+2) == CELL_EMPTY && y+2 <= MAP_HEIGHT-1)
				return 1;
			else
				return 0;

		case WEST:
			if (map_get_cell_type(map,x-1,y) == CELL_CASE &&
					map_get_cell_type(map,x-2,y) == CELL_EMPTY && x-2 >= 0)
				return 1;
			else
				return 0;

		case EAST:
			if (map_get_cell_type(map,x+1,y) == CELL_CASE &&
					map_get_cell_type(map,x+2,y) == CELL_EMPTY && x+2 <= MAP_WIDTH -1)
				return 1;
			else
				return 0;
	}
	return 0;
}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->current_direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}


struct player* player_from_save(int x, int y, int bomb_number,int key_number, int hp_number, int bomb_range, int current_direction){
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->x = x;
	player->y = y;
	player->nb_bomb = bomb_number;
	player->key = key_number;
	player->nb_hp = hp_number;
	player->bomb_range = bomb_range;
	player->time_attack=-5000; //default value
	player->current_direction = current_direction;

	return player;
}

void win_the_game(){
	window_display_image(sprite_get_win(), 2*40, 2*40);//display the winning image
	window_refresh();
	SDL_Delay(5000);
	exit(EXIT_SUCCESS);
}

void game_over(){
	window_display_image(sprite_get_game_over(), 0, 0);//display the gameover img
	window_refresh();
	SDL_Delay(5000);
	exit(EXIT_SUCCESS);
}
