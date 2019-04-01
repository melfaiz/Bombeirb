#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <constant.h>
#include <map.h>
#include <monster.h>
#include <bomb.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))

struct bomb{
	int bomb_x;
	int bomb_y;
	// time where the bomb is set
	int birth;
	int bomb_range;
	struct bomb *next;
	struct bomb *prev;
	//Replace the bomb_range in the 4 different direction if
	// an obstacle appears
	int block[4];
};

void set_bomb_position(int x,int y, struct bomb* bomb){
	assert(bomb);
	bomb->bomb_x = x;
	bomb->bomb_y = y;
}

int bomb_get_x( struct bomb* bomb){
	assert(bomb);
	return bomb->bomb_x;
}

int bomb_get_y( struct bomb* bomb){
	assert(bomb);
	return bomb->bomb_y;
}

void bomb_set_birth( struct bomb* bomb){
	assert(bomb);
	bomb->birth = SDL_GetTicks();
}
int bomb_get_birth( struct bomb* bomb){
	assert(bomb);
	return bomb->birth;
}

//block position to the map
int block_position(int position){
	if(position<0)
			position=0;
	else if (position>11)
			position=11;
	return position;
}


struct bomb* set_bomb_ad(struct player* player,struct map* map, struct bomb* game_bomb){
	int x = player_get_x(player);
	int y = player_get_y(player);
	struct bomb* new_bomb = malloc(sizeof(*new_bomb));

	new_bomb->next=NULL;
	new_bomb->prev=NULL;
	new_bomb->bomb_x=x;
	new_bomb->bomb_y=y;
	new_bomb->birth=SDL_GetTicks();
	new_bomb->bomb_range = player_get_bomb_range(player);
	// The new bomn will become the head of our bomb list
	//new_bomb->next=game_bomb;

	if (game_bomb != NULL){
		//
		game_bomb->prev=new_bomb;
		new_bomb->next=game_bomb;
	}

	map_set_cell_type(map, x, y, CELL_BOMB);
	player_dec_nb_bomb(player);
	return new_bomb;

}


void set_new_bomb_timer(struct bomb* bomb,int pause_time){
	int current_time = SDL_GetTicks();
	struct bomb* curent_bomb = bomb;
	while (curent_bomb != NULL){
		curent_bomb->birth = curent_bomb->birth + current_time - pause_time;
		curent_bomb = curent_bomb->next;
	}
}




void bomb_meca(struct bomb* bomb, struct map* map, struct player* player, struct monster** monster){
	if(bomb != NULL) {
		struct bomb* curent_bomb = bomb;
		struct bomb* tmp = malloc(sizeof(*tmp));
		    while (curent_bomb != NULL){
		    	int current_time = SDL_GetTicks();
		    	// We get the timer of the bomb
		    	int timer = (current_time - bomb_get_birth(curent_bomb));

				int x = bomb_get_x(curent_bomb);
				int y = bomb_get_y(curent_bomb);


				if (0<timer && timer<1000){
					map_set_type(map, x, y, BOMB4);
				}
				else if (1000<timer && timer<2000){
					map_set_type(map, x, y, BOMB3);
				}
				else if (2000<timer && timer<3000){
					map_set_type(map, x, y, BOMB2);
				}
				else if (3000<timer && timer<4000){
					map_set_type(map, x, y, BOMB1);

					//Check if there's two CELL_CASE side by side
					//If it's the case, loading the position of the first CELL_CASE
					//We do it just before the explosion cause otherwise the player could move the CELL_CASE on the map
					// And the real position of the case would be false
					// It's for also the
					if(timer>3950){
						curent_bomb->block[SOUTH]=block_fire(curent_bomb->bomb_range, map, x, y, SOUTH);
						curent_bomb->block[NORTH]=block_fire(curent_bomb->bomb_range, map, x, y, NORTH);
						curent_bomb->block[EAST]=block_fire(curent_bomb->bomb_range, map, x, y, EAST);
						curent_bomb->block[WEST]=block_fire(curent_bomb->bomb_range, map, x, y, WEST);

						destroy_bonus(curent_bomb->bomb_range, map, x, y,curent_bomb->block);

					}
				}
				else if (4000<timer && timer<6000){
					// ------- Management for the bomb location go the bomb at x and y ---------
					//set the fire where the bomb was
					map_set_type(map, x, y, CELL_FIRE);
					//Hurt the player if he is in a CELL_FIRE
					if (player_get_x(player) == x && player_get_y(player) == y){
						current_time = SDL_GetTicks();
						if ((current_time - player_get_time_attack(player)) > 2000){
							player_dec_nb_hp(player);
							player_set_time_attack(player,SDL_GetTicks());
						}
					}
					if(*monster != NULL)
						//if the monster is on the fire where the bomb was
						//his hp is set to 0
						set_hp0_monster(monster, x, y);
					// -------- END Management for the bomb location go the bomb at x and y ---------

					set_fire(map, x, y, player, timer, monster, curent_bomb->block, curent_bomb->birth, curent_bomb->bomb_range);

				}
				else if (timer>6000){
					stop_fire(curent_bomb->bomb_range,map,x,y);//end the explosion
					//Management of the bomb list
					if (curent_bomb->next == NULL && curent_bomb->prev != NULL){
						tmp = curent_bomb;
						(curent_bomb->prev)->next=NULL;
						free(tmp);
						//player_inc_nb_bomb(player);
					} // free the bomb

				}
				curent_bomb = curent_bomb->next;

			 }

		}
}

int block_fire(int bomb_range,struct map* map,int x, int y, enum direction direction){
	// As a precaution initialization of block_at is over bomb_range
	int block_at=10;
	x = block_position(x);
	y = block_position(y);
	if (direction == NORTH){
	for (int i=1; i<=bomb_range; i++){
		if ((map_get_cell_type(map, x, y-i) == CELL_CASE) ||
				(map_get_cell_type(map, x, y-i) == CELL_SCENERY) ||
				(map_get_cell_type(map, x, y-i) == CELL_DOOR)){
			block_at = i;
			break;
		}
	}
	}else if (direction == EAST){

	for (int j=1; j<=bomb_range; j++){
			if ((map_get_cell_type(map, x+j, y) == CELL_CASE) ||
				(map_get_cell_type(map, x+j, y) == CELL_SCENERY) ||
				(map_get_cell_type(map, x+j, y) == CELL_DOOR)){
				block_at = j;
				break;
			}
		}
	}else if (direction == SOUTH){

	for (int k=1 ; k<=bomb_range; k++){
			if ((map_get_cell_type(map, x, y+k) == CELL_CASE) ||
					(map_get_cell_type(map, x, y+k) == CELL_SCENERY) ||
					(map_get_cell_type(map, x, y+k) == CELL_DOOR)){
				block_at = k;
				break;
			}
		}
	}else if (direction == WEST){

	for (int l=1; l<=bomb_range; l++){
			if ((map_get_cell_type(map, x-l, y) == CELL_CASE) ||
					(map_get_cell_type(map, x-l, y) == CELL_SCENERY) ||
					(map_get_cell_type(map, x-l, y) == CELL_DOOR)){
				block_at = l;
				break;
			}
		}
	}
	return block_at;
}

void destroy_bonus(int bomb_range, struct map* map, int x, int y, int block[4]){
		for (int j=0; j<=MIN(block[SOUTH],bomb_range); j++){
			destroy_bonus_xy(map, x, y+j);
		}
		for (int j=0; j<=MIN(block[NORTH],bomb_range); j++){
			destroy_bonus_xy(map, x, y-j);
		}
		for (int j=0; j<=MIN(block[EAST],bomb_range); j++){
			destroy_bonus_xy(map, x+j, y);
		}
		for (int j=0; j<=MIN(block[WEST],bomb_range); j++){
			destroy_bonus_xy(map, x-j, y);
		}
}


void destroy_bonus_xy(struct map* map, int x, int y){
	x = block_position(x);
	y = block_position(y);
	if (map_get_cell_type(map, x, y) == CELL_BONUS)
		map_set_cell_type(map, x, y, CELL_EMPTY);
}


void set_fire(struct map* map, int x, int y, struct player* player,
				int timer, struct monster** monster,int block[4], int birth, int bomb_range){

	//Deal with a whole row/column at once
	//If the bombe explosion has to face an obstacle, it stop at the minimum between block[DIRECTION] and bomb_range
	for (int i=1; i<=MIN(block[SOUTH],bomb_range); i++){
		set_fire_xy(birth, map, x, y+i, player,monster);
	}

	for (int i=1; i<=MIN(block[NORTH],bomb_range); i++){
		set_fire_xy(birth, map, x, y-i, player,monster);
	}

	for (int i=1; i<=MIN(block[EAST],bomb_range); i++){
		set_fire_xy(birth, map, x+i, y, player,monster);
	}

	for (int i=1; i<=MIN(block[WEST],bomb_range); i++){
		set_fire_xy(birth, map, x-i, y, player,monster);
	}
}

//Deal with an element in position x y
void set_fire_xy(int birth, struct map* map, int x, int y, struct player* player, struct monster** monster){
	int type = map_get_cell_type(map, x, y);
	int current_time;

	x = block_position(x);
	y = block_position(y);

	//if fire is on a monster, the hp is set to zero
	//the function delete_monster free this monster
	if(*monster != NULL)
		set_hp0_monster(monster, x, y);

	switch(type){

	case CELL_EMPTY:
		current_time = SDL_GetTicks();
		// we display the fire one time
			if(current_time - birth - 4000 < 100){
				map_set_cell_type(map, x, y, CELL_FIRE);
			}
		break;

	case CELL_CASE:
		// the case is destroyed and its related bonus is set
		map_set_type(map, x, y, CELL_BONUS | map_get_sub_type(map, x, y));
		// If it's a monster, we add the monster to the list of game->monster
		if ((CELL_CASE | map_get_sub_type(map, x, y)) == CELL_CASE_MONSTER){
			// the monster appear from a case
			// a double pointer must be use
			p_set_monster_ad(monster, map, x, y);
		}
		break;

	case CELL_PLAYER:
		// We decrease the player life. Its life can't be decreased during 2000ms after being hurt
		// It's considered just after the explosion, after this is the CELL_FIRE which decrease the life.
		current_time = SDL_GetTicks();
		if(current_time - birth - 4000 < 100){

					if ((current_time - player_get_time_attack(player)) > 2000){
						player_dec_nb_hp(player);
						player_set_time_attack(player,SDL_GetTicks());
					}
		}
	    break;
	default:
		break;

	}

}

void stop_fire(int bomb_range, struct map* map, int x, int y){
		for (int i=0; i<=bomb_range; i++){
			// (map_get_cell_type(map, x, y+i) == CELL_MONSTER) is add to put a cell empty whan the monster is dead.
			if ((map_get_cell_type(map, x, y+i) == CELL_FIRE) || (map_get_cell_type(map, x, y+i) == CELL_MONSTER))
				map_set_cell_type(map, x, y+i, CELL_EMPTY);
			if ((map_get_cell_type(map, x, y-i) == CELL_FIRE) || (map_get_cell_type(map, x, y-i) == CELL_MONSTER))
				map_set_cell_type(map, x, y-i, CELL_EMPTY);
			if ((map_get_cell_type(map, x+i, y) == CELL_FIRE) || (map_get_cell_type(map, x+i, y) == CELL_MONSTER))
				map_set_cell_type(map, x+i, y, CELL_EMPTY);
			if ((map_get_cell_type(map, x-i, y) == CELL_FIRE) || (map_get_cell_type(map, x-i, y) == CELL_MONSTER))
				map_set_cell_type(map, x-i, y, CELL_EMPTY);
		}

}
