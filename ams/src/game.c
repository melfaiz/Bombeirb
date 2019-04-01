#include <assert.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <player.h>
#include <monster.h>
#include <bomb.h>
#include <map.h>

struct game {
	struct map* map; // the game's map
	struct player* player;
	struct monster* monster;
	struct bomb* bomb;
};

struct game* game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	// Initialization
	game->map = map_get('1');
	game->player = player_init(5,0,2,3);
	player_from_map(game->player, game->map); // get x,y of the player on the first map
	game->monster = NULL;
	game->bomb = NULL;
	game->monster=monster_from_map(game->monster, game->map);

	return game;
}

void game_next(struct game* game){
	//Needed to free when changes the level
	if (player_get_level_game_up(game->player) == 1){
		free(game->monster);
		free(game->bomb);
		game->monster=NULL;
		game->bomb=NULL;
		game->monster=monster_from_map(game->monster, game->map);
		player_set_pass_level_to_0(game->player);
	}

}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	map_free(game->map);
	free(game->bomb);
	free(game->monster);
}

struct map* game_get_map(struct game* game) {
	assert(game);
	return game->map;
}

struct bomb* game_get_bomb(struct game* game) {
	assert(game);
	return game->bomb;
}

struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

struct monster* game_get_monster(struct game* game){
	assert(game);
	return game->monster;
}

void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = game_get_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = 0;
	window_display_image(sprite_get_number(map_get_level(map)),x,y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_hp(game_get_player(game))), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_bomb_range(game_get_player(game))), x, y);

	//Display the key in the banner
	if (player_has_key(game_get_player(game))){
		x = 4 * white_bloc + 5 * SIZE_BLOC;
		window_display_image(sprite_get_banner_key(), x, y);
	}
}


void game_display(struct game* game) {
	assert(game);
	window_clear();

	//include here to have the good window_refresh for monsters
	game_meca(game);
	game_banner_display(game);
	map_display(game->map);
	player_display(game->player);
	window_refresh();
}

void game_meca(struct game* game){
		assert(game);
		//window_clear();
		game_next(game);

		bomb_meca(game->bomb, game->map, game->player, &(game->monster) );

		//Store the monsters in a temporary pointer
		struct monster* tmp = game->monster;

		//Take care of all monsters one by one to have independant moves
		while(tmp!=NULL){
			monster_move(tmp, game->map, game->player);
			// Added in order to save the monster born from the case as well
			map_set_cell_type(game->map,monster_get_x(tmp),monster_get_y(tmp), CELL_MONSTER);
			//Display monsters with appropriaye direction
			window_display_image(sprite_get_monster(monster_get_current_way(tmp)),
					monster_get_x(tmp)*40,monster_get_y(tmp)*40);
			tmp = monster_get_next(tmp);
		}
		//game->monster = tmp;
		game->monster = delete_monster(game->monster);
}

short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_map(game);
	struct bomb* bomb = game_get_bomb(game);
	struct monster* monster = game_get_monster(game);
	int done=0;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
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
				if (player_get_nb_bomb(player)>0)
					game->bomb=set_bomb_ad(player,map,bomb);
				break;
			case SDLK_p:
				window_display_image(sprite_get_pause(), 3*40, 3*40);
				window_refresh();
				int pause_time;
				pause_time = SDL_GetTicks();
				while(!done){
					done = pause_game();
				}
				//Handle the pause of bomb timers and monster timers
				set_new_monster_timer(monster,pause_time);
				set_new_bomb_timer(bomb,pause_time);
				break;
			case SDLK_s:
				save_game(game);
				window_display_image(sprite_get_saving(), 4*40, 6*40);
				window_refresh();

				int saving_time;
				saving_time = SDL_GetTicks();
				SDL_Delay(1100);
				//Handle the pause of bomb timers and monster timers
				set_new_monster_timer(monster,saving_time);
				set_new_bomb_timer(bomb,saving_time);
				break;
			case SDLK_l:
				load_game(game);
				window_display_image(sprite_get_loading(), 4*40, 6*40);
				window_refresh();

				int loading_time;
				loading_time = SDL_GetTicks();
				SDL_Delay(1100);
				//Handle the pause of bomb timers and monster timers
				set_new_monster_timer(monster,loading_time);
				set_new_bomb_timer(bomb,loading_time);

				game_display(game);
				window_refresh();

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
	if (input_keyboard(game))
		return 1; // exit game

	return 0;
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

void save_game(struct game* game){
	struct player* player = game_get_player(game);
	struct map* map = game_get_map(game);

	FILE* fichier = NULL;
	fichier = fopen("Save/Save", "r+"); //Save the info of the game in the file "Save"

	int width = map_get_width(map);
	int height = map_get_height(map);
	int map_level = map_get_level(map);
	unsigned char* grid = map_get_full_grid(map); //Load the current state of the map

	int x = player_get_x(player);
	int y = player_get_y(player);
	int current_direction = player_get_current_direction(player);
	int nb_bomb = player_get_nb_bomb(player);
	int key = player_has_key(player);
	int nb_hp = player_get_nb_hp(player);
	int bomb_range = player_get_bomb_range(player);

	fprintf(fichier,"%d \n",map_level);
	fprintf(fichier,"%d \n",x);
	fprintf(fichier,"%d\n",y);
	fprintf(fichier,"%d\n",current_direction);
	fprintf(fichier,"%d\n",nb_bomb);
	fprintf(fichier,"%d\n",key);
	fprintf(fichier,"%d\n",nb_hp);
	fprintf(fichier,"%d\n",bomb_range);
	fclose(fichier);

	fichier = fopen("data/map_s", "r+"); // Save the map in a different file (map_s)
	fprintf(fichier,"%d:%d \n", width, height);


	//Print each value of the map in the file
	int i, j , number;
		for (j = 0; j < height; j++){
			for (i = 0; i < width; i++){
				int N = i + j * width;
				if ( map_get_cell_type(map,i,j) == CELL_BOMB )
					grid [N] = CELL_EMPTY;
				if ( map_get_cell_type(map,i,j) == CELL_FIRE )
					grid [N] = CELL_EMPTY;
				number = grid[N] ;
				fprintf(fichier, "%d " , number);
			}
			fprintf(fichier, " \n");
		}

	fclose(fichier);
}

void load_game(struct game* game){
	struct player* player = malloc(sizeof(player));
	struct map* map  = malloc(sizeof(map));

	FILE* fichier = NULL;
	fichier = fopen("Save/Save", "r+"); // Load the file with all the game info

	int map_level;
	int x;
	int y;
	int current_direction;
	int nb_bomb;
	int key;
	int nb_hp;
	int bomb_range;

	//Extract the data from the file
	fscanf(fichier,"%d",&map_level);
	fscanf(fichier,"%d",&x);
	fscanf(fichier,"%d",&y);
	fscanf(fichier,"%d",&current_direction);
	fscanf(fichier,"%d",&nb_bomb);
	fscanf(fichier,"%d",&key);
	fscanf(fichier,"%d",&nb_hp);
	fscanf(fichier,"%d",&bomb_range);
	fclose(fichier);

	map = map_get('s'); //Load the map from a different file (map_s)
	map_set_maplvl(map,map_level);

	game->map = map;
	game->player = player_from_save(x, y, nb_bomb, key, nb_hp, bomb_range, current_direction);
	game->monster = NULL;
	game->bomb = NULL;
	game->monster=monster_from_map(game->monster, game->map);
}
