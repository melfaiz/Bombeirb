#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <constant.h>
#include <map.h>
#include <constant.h>

struct monster{
	int x;
	int y;
	enum direction current_direction;
	int speed;
	int time;
	short hp;
	int birth_time;
	struct monster* next;

};


struct monster* monster_get_next(struct monster* monster){
	return monster->next;
}

int monster_get_current_way(struct monster* monster){
	assert(monster);
	return	monster->current_direction;
}

void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->current_direction = way;
}

int monster_get_x(struct monster* monster){
	assert(monster);
	return monster->x;
}

int monster_get_y(struct monster* monster){
	assert(monster);
	return monster->y;
}

void set_new_monster_timer(struct monster* monster,int pause_time){
	assert(monster);
	int current_time = SDL_GetTicks();
	struct monster* current_monster = monster;
	while (current_monster != NULL){
		current_monster->time = current_monster->time + current_time - pause_time;
		current_monster = current_monster->next;
	}
}

struct monster* set_monster_ad(struct monster* monster, struct map* map, int x, int y) {
	assert(map);
	struct monster* new_monster = malloc(sizeof(*monster));
	new_monster->x = x;
	new_monster->y = y;
	new_monster->current_direction = WEST;
	new_monster->speed=1+0.2*map_get_level(map); // Set the speed of monsters according to the map level
	new_monster->time=SDL_GetTicks();
	new_monster->hp=1;
	new_monster->next = NULL;
	new_monster->birth_time=SDL_GetTicks();
	if (monster==NULL) // if the game has no monsters yet
		return new_monster;
	else{
		struct monster* tmp = monster;
		while (tmp->next != NULL)
			tmp=tmp->next;
		tmp->next=new_monster; // we add the monster at the end of list
	}
	return monster;
}

struct monster* monster_from_map(struct monster* monster, struct map* map) {
	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
	  for (j = 0; j < map_get_height(map); j++) {
	    if (map_get_cell_type(map, i, j) == CELL_MONSTER) {
	    	monster=set_monster_ad(monster, map,  i, j);
	    }
	  }
	}
	return monster;
}

void p_monster_init(struct monster** p_monster) {
		*(p_monster) = malloc(sizeof(struct monster*));
	}

void p_set_monster_ad(struct monster** monster, struct map* map, int x, int y) {
	assert(map);
	if ((*monster) == NULL){
		p_monster_init(monster);
	    (*monster)->x = x;
	  	(*monster)->y = y;
	    (*monster)->current_direction = WEST;
	  	(*monster)->speed=1+0.2*map_get_level(map); // Set the speed of monsters according to the map level
	  	(*monster)->time=SDL_GetTicks();
	  	(*monster)->hp = 1;
	  	(*monster)->birth_time=SDL_GetTicks();
	  	(*monster)->next = NULL;
	}
	else{
		*monster = set_monster_ad(*monster, map, x, y);
	}
}

// It's a double pointer because this function is called in bomb_meca() which is a void function
void set_hp0_monster(struct monster** monster, int x, int y){
	// x and y are the position of a CELL_FIRE
	struct monster* tmp = *monster; // temporary pointer on *monster
	int current_time = SDL_GetTicks();
	// We look all the monster
	while (tmp != NULL){
		// The monster has to remain invincible for 2 sec so he can't be killed by the explosion
		int timer = current_time - tmp->birth_time;
		// If the monster is on a CELL_FIRE he dies
		if (tmp->x == x && tmp->y == y && (timer > 2000)){
			tmp->hp=0;
		}
		tmp = tmp->next;
	}
}

struct monster* delete_monster(struct monster* monster){
	    //If the list is already empty
	    if(monster == NULL)
	        return NULL;
	    //if the monsters hp =0
	    if(monster->hp == 0){
	        //Delete the element but keep the address of the next element in a temporary pointer
	        struct monster* tmp = monster->next;
	        free(monster);
	        // Now the list begins with the next element
	        tmp = delete_monster(tmp);
	        return tmp;
	    }else{
	        // If the monster is okay, check for the next elements
	        monster->next = delete_monster(monster->next);
	        return monster;
	    }
}

int monster_move_aux(struct monster* monster, struct map* map, int x, int y) {
	if (!map_is_inside(map, x, y))
		return 0;


	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;
		break;

	case CELL_CASE:
		return 0;
		break;

	case CELL_BONUS:
		return 0;
		break;


	case CELL_PLAYER:
		return 1;
		break;

	case CELL_DOOR:
		return 0;
		break;

	case CELL_KEY:
		return 0;
		break;

	case CELL_FIRE:
		return 1;
		break;

	case CELL_MONSTER:
		return 0;
		break;
	default:
		break;
	}


	// Monster has moved
	return 1;
}

//function to get a random number for the random direction of the monsters
int rand_a_b(int a, int b){
    return rand()%(b-a) +a;
}

// If the monster touch the player it decrease player's life
void player_attacked(struct player* player, struct monster* monster){
	int current_time = SDL_GetTicks();
	if (player_get_x(player) == monster_get_x(monster) && player_get_y(player) == monster_get_y(monster)){
		if ((current_time - player_get_time_attack(player)) > 5000){
			player_dec_nb_hp(player);
			player_set_time_attack(player,SDL_GetTicks());
		}
	}
}

int monster_move(struct monster* monster, struct map* map, struct player* player) {
	int x = monster->x;
	int y = monster->y;
	int move = 0;
	int current_time = SDL_GetTicks();
	int timer = (current_time - (monster->time))*(monster->speed);// coefficient to make the monster quicker than before

	if (timer>3000){
		monster->current_direction = rand_a_b(0,4); // Random movement every 3seconds
		switch (monster->current_direction) {
		case NORTH:
			if (monster_move_aux(monster, map, x, y - 1) && y > 0) {
				map_set_cell_type(map, x, y , CELL_EMPTY);
				monster->y--;
				player_attacked(player, monster); // check in the function
				move = 1;
				monster->time=SDL_GetTicks();
				break;
			}
			break;

		case SOUTH:
			if (monster_move_aux(monster, map, x, y + 1) && y < MAP_HEIGHT-1) {
				map_set_cell_type(map, x, y , CELL_EMPTY);
				monster->y++;
				player_attacked( player, monster);
				move = 1;
				monster->time=SDL_GetTicks();
				break;
			}
			break;

		case WEST:
			if (monster_move_aux(monster, map, x - 1, y) && x > 0) {
				map_set_cell_type(map, x, y , CELL_EMPTY);
				monster->x--;
				player_attacked( player, monster);
				move = 1;
				monster->time=SDL_GetTicks();
				break;
					}
			break;

		case EAST:
			if (monster_move_aux(monster, map, x + 1, y) && x < MAP_WIDTH-1) {
				map_set_cell_type(map, x, y , CELL_EMPTY);
				monster->x++;
				player_attacked( player, monster);
				move = 1;
				monster->time=SDL_GetTicks();
				break;
			}
			break;

		}

	}//timer

	if (move) {
		map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
	}

	return move;

}



