#pragma once

#define MAX_UNITWORDS 64

typedef enum {
 // early era
 UNIT_FOOTMEN,
 UNIT_ARCHER,
 UNIT_HORSEMEN,
 
 UNIT_ENGINEER,
} unit_e;

typedef enum {
 ORDER_MOVE,
 ORDER_BUILD,
 ORDER_ATTACK,
} order_e;

typedef enum {
 ONSPOT,
 NORTH,
 NORTHWEST,
 SOUTHWEST,
 SOUTH,
 SOUTHEAST,
 NORTHEAST,
} aim_e;

typedef struct {
 order_e type;
 aim_e aim;
} order_t, intent_t;

typedef struct {
 unit_e type;
 int fealty; // state
 int number; // mancount
 int extra; // extra information and data
 
 order_t given;
} unit_t;

typedef struct {
 void* texture;
 int count;
} unitword_t;

extern unitword_t unitwords[];
