#pragma once

#define MAX_HOLDWORDS 32

typedef enum {
 HOLDING_NONE = 0,
 
 HOLDING_SEAT, // seats may not be relocated in wartime
 HOLDING_SETTLED,
 HOLDING_FARM,
 HOLDING_DWELLING,
 HOLDING_SHOP,
 HOLDING_DEFENSE,
 HOLDING_FORT,
 
 HOLDING_ROAD,
 HOLDING_PORT,
 
 // secret
 
 HOLDING_SORCERERS, // sorcerer's tower
} holding_e;

typedef struct {
 void* texture;
} holdword_t;

int holdingworth();
int holdingcoins();

int holdingoutput(); // stock output type
int holdingproduced(int stock); // production count
