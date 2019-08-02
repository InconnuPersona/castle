#pragma once

#include "holding.h"
#include "state.h"

#define MAX_COLUMNS 128
#define MAX_ROWS 128
#define MAX_STATES 16
#define MAX_HOLDINGS 64
#define MAX_LANDWORDS 32
#define MAX_LAYWORDS 32
#define MAX_UNITS 128

#define NO_CLAIMANT 0

#define TRANSLATE(Level, X, Y) \
 Level->tiles[Y * Level->rows + X]

typedef unsigned char byte_t;

typedef enum {
 DATA_TILE = 0,
 DATA_UNDEFINED,
 DATA_CLAIMANT, // whose rightful territory it is
 DATA_HOLDER, // current controlling state
 DATA_HOLDING, // holding
 DATA_STATIONED, // unit stationed here; same state as holder
 DATA_DEVELOPMENT,
 DATA_RESOURCE,
 
 DATA_ALL
} landdata_e;

typedef enum {
 LAND_INEXISTANT = 0,
 LAND_NONE = 0, // void
 
 LAND_MERE,
 LAND_PLAIN,
 LAND_HILL,
 LAND_MOUNTAIN,

/*LAND_COVERED,
 LAND_LUSH,
 LAND_VEGETATIVE,
 LAND_ARID,
 LAND_DRY,
 LAND_BOG,*/
} land_e;

typedef enum {
 SEASON_SPRING = 0,
 SEASON_SUMMER,
 SEASON_AUTUMN,
 SEASON_WINTER,
} season_e;

typedef enum {
 LAY_NONE,
 LAY_OUTLINE, // virtual layover tile; nonexistant
 LAY_VISIBLE,
 LAY_FOGOFWAR, // fog of war
 LAY_SELECT,
 LAY_SHADED, // rendered over other tiles when moving troops
} layover_e;

typedef struct {
 byte_t id, undefined, claimant, holder;
 byte_t holding, stationed, development, resource;
} land_t;

typedef union {
 land_t desc; // descriptor
 byte_t data[DATA_ALL];
} land_u;

typedef struct {
 void* texture;
 float debuff;
} landword_t;

typedef struct {
 int queue, turns;
 int columns, rows;
 
 land_u* tiles;
 
 state_t states[MAX_STATES];
 unit_t units[MAX_UNITS];
} level_t;

extern int statepalette[];
extern landword_t landwords[];

void setup_lands();

void init_level(level_t* level, int, int);
void generate(level_t* level, int states);

void ticklevel(level_t* level);
void reviselevel(level_t* level);
void renderlevel(level_t* level, int tx, int ty);

// TODO: implement functions
void updatetile(int index, level_t* level); // update a single tile
void updatestate(int index, level_t* level); // update a select state
void updatemap(level_t* level); // send a full update

void placestate(int state, level_t* level);
int assignplayerstate(level_t* level); // give a random state to the player

int grantstate(level_t* level);

int calcstateincome(int state, level_t* level);

void setturn(); // simulate one tile turn
