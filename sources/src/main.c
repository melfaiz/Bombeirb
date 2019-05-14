/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL.h>

#include <constant.h>
#include <game.h>
#include <window.h>
#include <misc.h>
#include <sprite.h>


int main(int argc, char *argv[]) {

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		error("Can't init SDL:  %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	struct game* game = game_new();

	struct map* map = game_get_current_map(game);
	
	int w = map_get_width(map);
	int h = map_get_height(map);


	window_create(SIZE_BLOC * w,SIZE_BLOC * h + BANNER_HEIGHT + LINE_HEIGHT);
	
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;
	
	int t1=SDL_GetTicks();
	int t2=SDL_GetTicks();

	window_display_image(sprite_get_menu(), 0,0);

	window_refresh();

	 
SDL_Event event;

 while ( SDL_WaitEvent(&event) >= 0 ) {
        switch (event.type) {
            case SDL_QUIT: 
                printf("Quit requested, quitting.\n");
                exit(0);
            
            break;

			case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							return 1;
						case SDLK_RETURN:
							goto game;
							break;
						default:
							break;
						}
			break;
        }


}
	int done;
	game :

	// game loop
	// static time rate implementation
	done = 0;
	while (!done) {
		timer = SDL_GetTicks();

		done = game_update(game);
		game_display(game);

		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.


		// game_speed_one() s'executera donc une fois par seconde
		if (SDL_GetTicks()-t1>1000){
			game_speed_one(game);
			t1= SDL_GetTicks();
		} 

		// 1000- 200*game_get_current_lvl(game) est une bijection de 10 niveaux vers une periode entre 0.3s et 1s
		struct player* player = game_get_player(game);
		if ( SDL_GetTicks()-t2 > (1000- 70*player_get_lvl(player) )){
			game_speed_lvl(game);
			t2= SDL_GetTicks();
		} 

	}

	game_free(game);

	SDL_Quit();

	return EXIT_SUCCESS;
}