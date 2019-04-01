#ifndef MAP_H_
#define MAP_H_




enum cell_type {
	CELL_EMPTY=0,
	CELL_SCENERY,  	// 1	0001
	CELL_PLAYER, 	// 2	0010
	CELL_PRINCESS, 	// 3	0011
	CELL_CASE,   	// 4	0100
	CELL_BONUS, 	// 5	0101
	CELL_MONSTER, 	// 6	0110
	CELL_BOMB, 		// 7	0111
	CELL_KEY, 		// 8	1000
	CELL_DOOR, 		// 9	1001
	CELL_FIRE,		//10 	1010	new
};

enum bonus_type {
	BONUS_BOMB_RANGE_DEC=1, // 0001
	BONUS_BOMB_RANGE_INC, 	// 0010
	BONUS_BOMB_NB_DEC, 		// 0011
	BONUS_BOMB_NB_INC, 		// 0100
	BONUS_MONSTER, 			// 0101
	BONUS_LIFE 				// 0110
};

enum scenery_type {
	SCENERY_TREE,   // 0
	SCENERY_STONE,  // 1
};

enum compose_type {
	CELL_TREE = CELL_SCENERY | (SCENERY_TREE << 4), 				//  0000 0001
	CELL_STONE  = CELL_SCENERY | (SCENERY_STONE << 4), 				//  0001 0001

	/////////////////////modif INC en DEC
    CELL_CASE_RANGEDEC = CELL_CASE | (BONUS_BOMB_RANGE_DEC << 4), 	//  0001 0100
    CELL_CASE_RANGEINC = CELL_CASE | (BONUS_BOMB_RANGE_INC << 4), 	//  0010 0100
	CELL_CASE_BOMBDEC  = CELL_CASE | (BONUS_BOMB_NB_DEC << 4), 		//  0011 0100
    CELL_CASE_BOMBINC  = CELL_CASE | (BONUS_BOMB_NB_INC << 4), 		//  0100 0100
    CELL_CASE_LIFE     = CELL_CASE | (BONUS_LIFE << 4), 			//  0110 0100
    CELL_CASE_MONSTER  = CELL_CASE | (BONUS_MONSTER << 4),			//  0101 0100
			 
};

enum bomb_state{
	WICK1 = 1,
	WICK2,
	WICK3,
	WICK4,
};

enum bomb_type {//3.3
	BOMB1 = CELL_BOMB | ( WICK1 << 4),  //0000 0111
	BOMB2 = CELL_BOMB | ( WICK2 << 4),  //0001 0111
	BOMB3 = CELL_BOMB | ( WICK3 << 4),  //0010 0111
	BOMB4 = CELL_BOMB | ( WICK4 << 4),  //0011 0111
	
};
struct map;

// Create a new empty map
struct map* map_new(int width, int height);
void map_free(struct map* map);


// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the Cell i,j
int map_get_grid(struct map* map, int i, int j);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);
enum cell_type map_get_sub_type(struct map* map, int x, int y);

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);

// Set one of the 4 state of the bomb according to the type
void  map_set_type(struct map* map, int x, int y, enum bomb_type type);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default 12x12 static map
struct map* map_get_default();

// Display the map on the screen
void map_display(struct map* map);

// Return map_N
struct map* map_get(char N);

//Return 1 if the door is open, 0 if closed
int door_open(int x, int y,struct map* map);

//return the level of the next map
char next_level(struct map* map);

//return the level of the current map
int map_get_level(struct map* map);

//change the map level
void map_change_level(struct map* map,int maplvl);

//return the full grid of the map
unsigned char* map_get_full_grid(struct map* map);

//Set the map_level of the map according to int map_level
struct map* map_set_maplvl(struct map* map, int map_level);

#endif /* MAP_H_ */
