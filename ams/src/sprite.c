#include <SDL/SDL_image.h>
#include <assert.h>

#include <sprite.h>
#include <misc.h>
#include <constant.h>

// Sprites general
#define MAP_CASE        "sprite/wood_box.png"
#define MAP_KEY			"sprite/key.png"
#define CLOSED 			"sprite/closed_door.png"
#define MAP_DOOR_OPENED	"sprite/door.png"


// Scenery elements
#define MAP_STONE		"sprite/stone.png"
#define MAP_TREE        "sprite/tree.png"

// Sprites of Banner
#define BANNER_LINE		"sprite/banner_line.png"
#define BANNER_LIFE		"sprite/banner_life.png"
#define BANNER_BOMB		"sprite/bomb4.png"
#define BANNER_RANGE	"sprite/banner_range.png"
#define BANNER_KEY      "sprite/banner_key.png"
#define BANNER_0		"sprite/banner_0.jpg"
#define BANNER_1		"sprite/banner_1.jpg"
#define BANNER_2		"sprite/banner_2.jpg"
#define BANNER_3		"sprite/banner_3.jpg"
#define BANNER_4		"sprite/banner_4.jpg"
#define BANNER_5		"sprite/banner_5.jpg"
#define BANNER_6		"sprite/banner_6.jpg"
#define BANNER_7		"sprite/banner_7.jpg"
#define BANNER_8		"sprite/banner_8.jpg"
#define BANNER_9		"sprite/banner_9.jpg"

// Sprites of Bombs
#define BOMB_TTL1       "sprite/bomb1.png"
#define BOMB_TTL2       "sprite/bomb2.png"
#define BOMB_TTL3       "sprite/bomb3.png"
#define BOMB_TTL4       "sprite/bomb4.png"

//Sprite of fire
#define FIRE			"sprite/fire.png"

// Sprites of Bonus
#define IMG_BONUS_BOMB_RANGE_INC  "sprite/bonus_bomb_range_inc.png"
#define IMG_BONUS_BOMB_RANGE_DEC  "sprite/bonus_bomb_range_dec.png"
#define IMG_BONUS_BOMB_NB_INC     "sprite/bonus_bomb_nb_inc.png"
#define IMG_BONUS_BOMB_NB_DEC     "sprite/bonus_bomb_nb_dec.png"
#define IMG_BONUS_LIFE            "sprite/life.png"

// Sprites of Players
#define PLAYER_LEFT     "sprite/player_left.png"
#define PLAYER_UP       "sprite/player_up.png"
#define PLAYER_RIGHT    "sprite/player_right.png"
#define PLAYER_DOWN     "sprite/player_down.png"

// Sprites of Monsters
#define MONSTER_LEFT	"sprite/monster_left.png"
#define MONSTER_UP		"sprite/monster_up.png"
#define MONSTER_RIGHT	"sprite/monster_right.png"
#define MONSTER_DOWN	"sprite/monster_down.png"

// Sprites of princess
#define PRINCESS		"sprite/bomberwoman.png"

//Sprite of Pause Button
#define PAUSE_BUTTON	"sprite/Pause2.png"

//Sprite of WINNING icon
#define WIN	"sprite/Win.gif"

//Sprite of GAMEOVER icon
#define GAME_OVER	"sprite/game_over.png"

//Sprite of laoding img
#define LOADING	"sprite/image_loading.png"

//Sprite of saving img
#define SAVING	"sprite/image_save.png"

// banner
SDL_Surface* numbers[10];
SDL_Surface* banner_life;
SDL_Surface* banner_bomb;
SDL_Surface* banner_range;
SDL_Surface* banner_line;
SDL_Surface* banner_key;

// map
SDL_Surface* box;
SDL_Surface* goal;
SDL_Surface* key;
SDL_Surface* door_closed;
SDL_Surface* door_opened;

SDL_Surface* stone;
SDL_Surface* tree;


//bomb
SDL_Surface* bomb_img[5];

//fire
SDL_Surface* fire;

// bonus
#define NB_BONUS 7
SDL_Surface* bonus[NB_BONUS];

// player
SDL_Surface* player_img[4];

// princess
SDL_Surface* princess;

//Pause button
SDL_Surface* pause_img;

//Winning icon
SDL_Surface* win_img;

//Game over icon
SDL_Surface* game_over_img;

//Loading image
SDL_Surface* loading_img;

//Saving image
SDL_Surface* saving_img;

// monsters
SDL_Surface* monsters_img[4];

void banner_load() {
	// numbers imgs
	numbers[0] = load_image(BANNER_0);
	numbers[1] = load_image(BANNER_1);
	numbers[2] = load_image(BANNER_2);
	numbers[3] = load_image(BANNER_3);
	numbers[4] = load_image(BANNER_4);
	numbers[5] = load_image(BANNER_5);
	numbers[6] = load_image(BANNER_6);
	numbers[7] = load_image(BANNER_7);
	numbers[8] = load_image(BANNER_8);
	numbers[9] = load_image(BANNER_9);

	// other banner sprites
	banner_life = load_image(BANNER_LIFE);
	banner_bomb = load_image(BANNER_BOMB);
	banner_range = load_image(BANNER_RANGE);
	banner_key = load_image(BANNER_KEY); //3.2.3
	banner_line = load_image(BANNER_LINE);
}

void banner_unload() {
	// numbers imgs
	for (int i = 0; i < 10; i++) {
		SDL_FreeSurface(numbers[i]);
	}

	// other banner sprites
	SDL_FreeSurface(banner_life);
	SDL_FreeSurface(banner_bomb);
	SDL_FreeSurface(banner_range);
	SDL_FreeSurface(banner_life);
	SDL_FreeSurface(banner_key);
}

void map_load() {
	// Sprite loading
	tree = load_image(MAP_TREE);
	box = load_image(MAP_CASE);
	key = load_image(MAP_KEY);
	stone = load_image(MAP_STONE);
	door_closed = load_image(CLOSED);
	door_opened = load_image(MAP_DOOR_OPENED);
	princess = load_image(PRINCESS);
	
}

void map_unload() {
	SDL_FreeSurface(door_closed);
	SDL_FreeSurface(tree);
	SDL_FreeSurface(box);
	SDL_FreeSurface(goal);
	SDL_FreeSurface(key);
	SDL_FreeSurface(stone);
	SDL_FreeSurface(door_opened);
	SDL_FreeSurface(princess);

}

void bonus_load() {
	bonus[0] = NULL;
	bonus[BONUS_BOMB_RANGE_INC] = load_image(IMG_BONUS_BOMB_RANGE_INC);
	bonus[BONUS_BOMB_RANGE_DEC] = load_image(IMG_BONUS_BOMB_RANGE_DEC);
	bonus[BONUS_BOMB_NB_INC] = load_image(IMG_BONUS_BOMB_NB_INC);
	bonus[BONUS_BOMB_NB_DEC] = load_image(IMG_BONUS_BOMB_NB_DEC);
	bonus[BONUS_MONSTER] = load_image(MONSTER_DOWN);
	bonus[BONUS_LIFE] = load_image(IMG_BONUS_LIFE);
}

void bonus_unload() {
	for (int i = 0; i < NB_BONUS+1; i++)
		if(bonus[i])
			SDL_FreeSurface(bonus[i]);
}

void bomb_load() {
	bomb_img[0] = NULL;
	bomb_img[WICK4] = load_image(BOMB_TTL4);
	bomb_img[WICK3] = load_image(BOMB_TTL3);
	bomb_img[WICK2] = load_image(BOMB_TTL2);
	bomb_img[WICK1] = load_image(BOMB_TTL1);

	fire = load_image(FIRE);
}

void bomb_unload() {
	for (int i = 0; i < 5; i++)
		SDL_FreeSurface(bomb_img[i]);

	SDL_FreeSurface(fire);

}
	
void player_load() {
	player_img[WEST] = load_image(PLAYER_LEFT);
	player_img[EAST] = load_image(PLAYER_RIGHT);
	player_img[NORTH] = load_image(PLAYER_UP);
	player_img[SOUTH] = load_image(PLAYER_DOWN);
}

void player_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(player_img[i]);
}

void monsters_load() {
	monsters_img[WEST] = load_image(MONSTER_LEFT);
	monsters_img[EAST] = load_image(MONSTER_RIGHT);
	monsters_img[NORTH] = load_image(MONSTER_UP);
	monsters_img[SOUTH] = load_image(MONSTER_DOWN);
}

void monsters_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(monsters_img[i]);
}

void Pause_load(){
	pause_img = load_image(PAUSE_BUTTON);
}

void Pause_unload(){
	SDL_FreeSurface(pause_img);
}

void win_load(){
	win_img = load_image(WIN);
}

void win_unload(){
	SDL_FreeSurface(win_img);
}

void game_over_load(){
	game_over_img = load_image(GAME_OVER);
}

void game_over_unload(){
	SDL_FreeSurface(game_over_img);
}


void loading_load(){
	loading_img = load_image(LOADING);
}
void loading_unload(){
	SDL_FreeSurface(loading_img);
}

void saving_load(){
	saving_img = load_image(SAVING);
}
void saving_unload(){
	SDL_FreeSurface(saving_img);
}

void sprite_load() {
	map_load();
	bonus_load();
	banner_load();
	player_load();
	bomb_load();
	monsters_load();
	Pause_load();
	win_load();
	game_over_load();
	saving_load();
	loading_load();
	
}

void sprite_free() {		
	map_unload();
	bonus_unload();
	banner_unload();
	player_unload();
	bomb_unload();
	monsters_unload();
	Pause_unload();
	win_unload();
	game_over_unload();
	saving_unload();
	loading_unload();
}

SDL_Surface* sprite_get_number(short number) {
	assert(number >= 0 && number < 9);
	return numbers[number];
}

SDL_Surface* sprite_get_player(enum direction direction) {
	assert(player_img[direction]);
	return player_img[direction];
}

SDL_Surface* sprite_get_monster(enum direction direction) {
	assert(monsters_img[direction]);
	return monsters_img[direction];
}

SDL_Surface* sprite_get_princess() {
	assert(princess);
	return princess;
}

SDL_Surface* sprite_get_banner_life() {
	assert(banner_life);
	return banner_life;
}

SDL_Surface* sprite_get_banner_bomb() {
	assert(banner_bomb);
	return banner_bomb;
}

SDL_Surface* sprite_get_banner_line() {
	assert(banner_line);
	return banner_line;
}

SDL_Surface* sprite_get_banner_range() {
	assert(banner_range);
	return banner_range;
}
//3.2.3
SDL_Surface* sprite_get_banner_key() {
	assert(banner_key);
	return banner_key;
}

SDL_Surface* sprite_get_bonus(enum bonus_type bonus_type) {
	assert(bonus[bonus_type]);
	return bonus[bonus_type];
}

SDL_Surface* sprite_get_tree() {
	assert(tree);
	return tree;
}

SDL_Surface* sprite_get_box() {
	assert(box);
	return box;
}

SDL_Surface* sprite_get_key() {
	assert(key);
	return key;
}

SDL_Surface* sprite_get_stone() {
	assert(stone);
	return stone;
}

SDL_Surface* sprite_get_bomb(enum bomb_state bomb_state) {
	assert(bomb_img[bomb_state]);
	return bomb_img[bomb_state];
}

SDL_Surface* sprite_get_fire() {
	assert(fire);
	return fire;
}

SDL_Surface* sprite_get_door_closed() {
	assert(door_closed);
	return door_closed;
}

SDL_Surface* sprite_get_door_opened() {
	assert(door_opened);
	return door_opened;
}

SDL_Surface* sprite_get_pause() {
	assert(pause_img);
	return pause_img;
}

SDL_Surface* sprite_get_win() {
	assert(win_img);
	return win_img;
}

SDL_Surface* sprite_get_game_over() {
	assert(game_over_img);
	return game_over_img;
}

SDL_Surface* sprite_get_loading() {
	assert(loading_img);
	return loading_img;
}

SDL_Surface* sprite_get_saving() {
	assert(saving_img);
	return saving_img;
}
