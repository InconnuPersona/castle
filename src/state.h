#pragma once

#include "courtaffair.h"
#include "unit.h"

#define MAX_STATE_UNITS 64
#define MAX_STATE_IDEAS 32
#define MAX_STATEHOLDERS 16
#define MAX_STOCKPILES 16
#define MAX_STATE_TREATIES 16

#define STATESTOCK(State, Stock) State.stockpile[STOCK_##Stock]

typedef enum {
 MENU_MAIN,
 MENU_NETWORK,
 MENU_LOBBY,
 MENU_OPTIONS,
 
 MENU_GAME,
} menu_e;

typedef enum {
 INEXTANT,
 STATE,
 TERRITORY
} state_e;

typedef enum {
 STOCK_PRECIOUSMETAL, // gold, silver, valuables, etc.
 STOCK_ORGANICS, // meat, vegetables, water, etc.
 STOCK_RESEARCH, // articles, research, knowledge, etc.
 STOCK_SIDEARMS, // essential handsweapons
 STOCK_FIREARMS, // manufactured weapons
 STOCK_CLOTHING, // essential clothing and temperature resistant materials
 STOCK_FORGEMETAL, // iron, copper, etc.
 STOCK_WRITING, // paper, ink, etc.
 STOCK_MATERIAL, // lumber, stone, etc.
 
 STOCK_ALL // last stockpile entry
} stockpile_e;

typedef struct {
 int stock;
 int value;
 int push, grab; // import and export of market good
} marketgood_t;

typedef enum {
 MARKET_ENABLED = 0x1,
 MARKET_IMPORT = 0x2,
 MARKET_EXPORT = 0x4,
 
 MARKET_ENABLEDIMPORT = MARKET_ENABLED | MARKET_IMPORT,
 MARKET_ENABLEDEXPORT = MARKET_ENABLED | MARKET_EXPORT,
} marketstate_e;

typedef struct {
 marketstate_e state;
 
 marketgood_t stocks[STOCK_ALL];
} market_t;

typedef struct {
 state_e type;
 int id, ai;
 
 market_t market, stockpile;
 
 int ideas[MAX_STATE_IDEAS]; // you may drop ideas to remove entire idea groups and retain a portion of the investment
 treaty_t treaties[MAX_STATE_TREATIES];
} state_t;

typedef struct {
 // contain network update information
} stateupdate_t;

typedef enum {
 OBJECT_TILE,
} object_e;

typedef enum {
 VECTOR_X,
 VECTOR_Y,
 VECTOR_INDEX,
} vector_e;

// translate designated object field word to proper value (state index to state level index)
int reword(int word, object_e object, vector_e vector);
