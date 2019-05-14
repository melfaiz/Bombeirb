/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <monster.h>

struct game {
     
	struct map** maps; // the game's maps
	short levels;        // nb maps of the game
	struct player* player;
		
};

struct game*
game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));

	game->player = player_init(3);

	game->levels = 8;

	game->maps = malloc(game->levels *sizeof(struct map*));

	for(int i=0;i< game->levels;i++){
		game->maps[i] = map_get(i +'0');
		map_set_player(game->maps[i],game->player);
	}
		

	// Set default location of the player
	player_set_position(game->player, 2, 2);

	


	return game;
}



void game_speed_one(struct game* game){
	

	struct player* player = game_get_player(game);
	int range = player_get_range(player);

	for (int i = 0; i < game->levels; i++)
	{
			struct map* map = game->maps[i];
			map_clear_explosion(map);
			map_bomb_dec(map,range);
	}



}

void game_speed_lvl(struct game* game){
	
	for (int i = 0; i < game->levels; i++)
	{		
		struct map* map = game->maps[i];
		monsters_move(map);
	}

}

void game_free(struct game* game) {
	assert(game);										
	
	player_free(game->player);

	for (int i = 0; i < game->levels; i++)
	{
		struct map* map = game->maps[i];
		map_free(map); 
	}
	
	
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	struct player* player = game_get_player(game);
	return game->maps[player_get_lvl(player)];
}



struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}



void game_banner_display(struct game* game) {
	assert(game);
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_lives(game_get_player(game))), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_range(player)), x, y);
}

void game_display(struct game* game) {
	assert(game);
	
	window_clear();

	game_banner_display(game);

	map_display(game_get_current_map(game));

	player_display(game->player);
	
	monsters_display(game_get_current_map(game));

	window_refresh();
}

static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);
	int pause=0;

	int w = map_get_width(map);
	int h = map_get_height(map);

	int i = player_get_x(game->player);
	int j = player_get_y(game->player);
	while (SDL_PollEvent(&event)) {

		switch (event.type){
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				player_move(player, map);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(player, map);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(player, map);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(player, map);
				break;
			case SDLK_SPACE:

						player_drop_bomb(player, map,i,j);
						
				break;

			case SDLK_p:

				window_display_image(sprite_get_pause(), SIZE_BLOC*(w/2 -1.5),SIZE_BLOC*(h/2 -1.5));
				window_refresh();

				while(!pause){
					pause = pause_game();
				}

			break;

			case SDLK_s:
				save_game(game);
				window_display_image(sprite_get_save(), 3*SIZE_BLOC, 5*SIZE_BLOC);
				window_refresh();

				SDL_Delay(1500);

				break;

			case SDLK_l:
				load_game(game);
				window_display_image(sprite_get_load(), 2.5*SIZE_BLOC, 5*SIZE_BLOC);
				window_refresh();

				SDL_Delay(1500);

				break;
			default:
				break;
			}
		break;
		}
	}
	return 0;
}




int game_update(struct game* game) {

	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);

	for (int i = 0; i < game->levels; i++)
	{	
		if(player_get_lvl(player) == i){
			map_bomb_player(map);
		}
			
		struct map* map = game->maps[i];
		map_bomb_monsters(map);
	}

	game_monster_get_player(game);
	monster_collision(game);



	if (input_keyboard(game) || end_game(player))
		return 1; // exit game

	return 0;
}


void monster_collision(struct game* game){

	struct player* player = game_get_player(game);

	struct map* map = game_get_current_map(game);

	int x_p = player_get_x(player);
	int y_p = player_get_y(player);

	struct monster** monsters = map_get_monsters(map);

	for(int i = 0; i < map_get_nb_monsters(map); i++)
	{
		if (is_monster_alive(monsters[i])){
			int x = monster_get_x(monsters[i]);
			int y = monster_get_y(monsters[i]);

				if (x_p == x && y_p == y) {
					player_down(player);

				}
		}
	}


}


int pause_game(){
	
	SDL_Event event;
	
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_p:
				return 1;
				break;
			default:
				break;
			}
		break;
		}
	}
	return 0;
}

int menu_game(){
	
	SDL_Event event;

	window_display_image(sprite_get_explosion(), 50,160);
	window_refresh();

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_m:
				return 1;
				break;
			case SDLK_ESCAPE:
				return 1;
				break;

			case SDLK_UP:
				printf("UP\n");
			break;

			default:
				break;
			}
		break;
		}
	}
	return 0;
}


void save_game(struct game* game){

	struct player* player = game_get_player(game);
	

	FILE* fichier = NULL;
	fichier = fopen("data/game_s", "r+"); 

	int level = player_get_lvl(player);
	int x = player_get_x(player);
	int y = player_get_y(player);
	int current_direction = player_get_current_way(player);
	int nb_bomb = player_get_nb_bomb(player);

	int lives = player_get_nb_lives(player);
	int range = player_get_range(player);

	fprintf(fichier,"%d \n",level);
	fprintf(fichier,"%d \n",x);
	fprintf(fichier,"%d\n",y);
	fprintf(fichier,"%d\n",current_direction);
	fprintf(fichier,"%d\n",nb_bomb);
	fprintf(fichier,"%d\n",lives);
	fprintf(fichier,"%d\n",range);
	fclose(fichier);


	fichier = fopen("data/maps_s", "r+"); 

	for (int m = 0; m < game->levels; m++)
	{
		struct map* map = game->maps[m];
		unsigned char* grid = map_get_grid(map);
		int h = map_get_height(map);
		int w = map_get_width(map);

		fprintf(fichier, "%d:%d\n" , w,h);
	
			int i, j , cell;
				for (j = 0; j < h; j++){
					for (i = 0; i < w; i++){

						int N = i + j * w;

						if ( map_get_cell_type(map,i,j) == CELL_BOMB || map_get_cell_type(map,i,j) == CELL_EXPLOSION ){
							cell = CELL_EMPTY;
						}
						if ( is_cell_monster(map,i,j) ){
							cell = CELL_MONSTER;
						}else
						{
							cell = grid[N];
						}
						
						fprintf(fichier, "%d " ,cell);
					}
					fprintf(fichier, " \n");
				}
			fprintf(fichier, "%d\n" , door_is_closed(map));
			fprintf(fichier, " \n");
			fprintf(fichier, " \n");
	}
	fclose(fichier);


}


void load_game(struct game* game){
	struct player* player = malloc(sizeof(player));
	struct map* map  = malloc(sizeof(map));

	FILE* fichier = NULL;
	fichier = fopen("data/game_s", "r+"); // Load the file with all the game info

	int level;
	int x;
	int y;
	int direction;
	int nb_bomb;

	int lives;
	int range;

	//Extract the data from the file
	fscanf(fichier,"%d",&level);
	fscanf(fichier,"%d",&x);
	fscanf(fichier,"%d",&y);
	fscanf(fichier,"%d",&direction);
	fscanf(fichier,"%d",&nb_bomb); 
	fscanf(fichier,"%d",&lives);
	fscanf(fichier,"%d",&range);
	fclose(fichier);


	set_player(game->player,x,y,level,direction,nb_bomb,lives,range) ;

	fichier = fopen("data/maps_s", "r+"); 

	for (int m = 0; m < game->levels; m++)
	{
		struct map* map = game->maps[m];


		int h ;
		int w ;
		int door_is_closed;

		
		//struct monster** monsters = map_get_monsters(map) ;
		
		//monsters = malloc(w*h * sizeof(struct monster*));

		map_set_nb_monsters(map,0);

		fscanf(fichier, "%d:%d\n" , &w,&h);
		
		unsigned char* grid = map_get_grid(map);


			int i, j;
				for (j = 0; j < h; j++){
					for (i = 0; i < w; i++){

						int N = i + j * w;

						fscanf(fichier, "%hhu " ,&grid[N]); ;
						
					}
					fscanf(fichier, " \n");
				}
				fscanf(fichier, "%d\n",&door_is_closed);
			fscanf(fichier, " \n");
			fscanf(fichier, " \n");

			if (!door_is_closed)
			{
				map_open_door( map);
			}
			
	}
	fclose(fichier);


}

void game_monster_get_player(struct game* game){

        struct player* player = game_get_player(game);
        struct map* map = game_get_current_map(game);
        struct monster** monsters = map_get_monsters(map);


        for(int i = 0; i < map_get_nb_monsters(map); i++)
        {


                if (is_monster_alive(monsters[i])) {

                        int x_m = monster_get_x(monsters[i]);
                        int y_m = monster_get_y(monsters[i]);
                        int x_p = player_get_x(player);
                        int y_p = player_get_y(player);

                        if (x_m != x_p ) {
                                if ( (x_m < x_p) && (monster_move_aux(monsters[i],map,x_m+1,y_m))) {
                                        monster_set_current_way( monsters[i], EAST );
                                } else if((x_m > x_p) && (monster_move_aux(monsters[i],map,x_m-1,y_m))) {
                                        monster_set_current_way( monsters[i], WEST );
                                } else{
                                        if ( y_m < y_p) {
                                                monster_set_current_way( monsters[i], SOUTH );
                                        } else if (y_m > y_p) {
                                                monster_set_current_way( monsters[i], NORTH );
                                                }
                                  }
                        } else if(y_m != y_p){
                                        if (( y_m < y_p) && (monster_move_aux(monsters[i],map,x_m,y_m+1)) ) {
                                                monster_set_current_way( monsters[i], SOUTH );
                                        } else if ((y_m > y_p) && (monster_move_aux(monsters[i],map,x_m,y_m-1)) ) {
                                        monster_set_current_way( monsters[i], NORTH );
                              }
                                else{
                                        if ( x_m < x_p) {
                                                monster_set_current_way( monsters[i], EAST );
                                        } else if (x_m > x_p) {
                                                monster_set_current_way( monsters[i], WEST );
                                                }

                                }

                   }
          }
  }
}