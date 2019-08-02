#include "gfx.h"
#include "net.h"

#include <stdlib.h>
#include <stdio.h>

#define lsoe(Value, Attribute) (Value <= Attribute ? Value : Attribute)

landword_t landwords[MAX_LANDWORDS] = { 0 };

texture_t laywords[MAX_LAYWORDS] = { 0 };
texture_t holdwords[MAX_HOLDWORDS] = { 0 };
unitword_t unitwords[MAX_UNITWORDS] = { 0 };

int statepalette[16] = {
 COLOR_RED,
 COLOR_GREEN,
 COLOR_BLUE,
 COLOR_MAGENTA,
 COLOR_CYAN,
 COLOR_YELLOW,
 COLOR_HALFRED,
 COLOR_HALFGREEN,
 COLOR_HALFBLUE
};

void loadland(int index, const char* texture, float debuff) {
 landwords[index].texture = loadtexture(texture);
 landwords[index].debuff = debuff;
}

void loadlay(int index, const char* texture) {
 laywords[index] = loadtexture(texture);
}

void loadhold(int index, const char* texture) {
 holdwords[index] = loadtexture(texture);
}

void loadunit(int index, const char* texture) {
 unitwords[index].texture = loadtexture(texture);
}

void setup_lands() {
 printf(" Some useless messages:\n");
 fflush(stdout);
 
 loadlay(LAY_OUTLINE, "res/outline.png");
 //loadlay(LAY_SHADOW, "res/shadow.png");
 
 loadland(LAND_PLAIN, "res/plain.png", 1);
 loadland(LAND_MERE, "res/mere.png", 5);
 loadland(LAND_HILL, "res/hill.png", 3);
 loadland(LAND_MOUNTAIN, "res/mountain.png", 5);
 
 loadhold(HOLDING_SEAT, "res/seat.png");
 loadhold(HOLDING_SETTLED, "res/settled.png");
 loadhold(HOLDING_DWELLING, "res/dwelling.png");
 loadhold(HOLDING_FARM, "res/farm.png");
 loadhold(HOLDING_SHOP, "res/shop.png");
 loadhold(HOLDING_FORT, "res/fort.png");
 loadhold(HOLDING_PORT, "res/port.png");
 
 loadunit(UNIT_FOOTMEN, "res/unit/footmen.png");
 loadunit(UNIT_ENGINEER, "res/unit/engineer.png");
}

void init_level(level_t* level, int rows, int columns) {
 int i;
 
 level->rows = lsoe(rows, MAX_ROWS);
 level->columns = lsoe(columns, MAX_COLUMNS);
 level->tiles = calloc(level->rows * level->columns, sizeof(land_u));
 
 for (i = 0; i < level->columns * level->rows; i++) {
  level->tiles[i].desc.id = LAND_NONE;
  level->tiles[i].data[DATA_HOLDER] = NO_CLAIMANT;
  level->tiles[i].data[DATA_HOLDING] = HOLDING_NONE;
 }
 
 for (i = 0; i < MAX_STATES; i++) {
  level->states[i].type = INEXTANT;
 }
 
 level->queue = givequeue("level");
 
 return;
}

void generate(level_t* level, int states) {
 int i, x, y;
 
 // land sea generation; equivalent of a heightmap filter
 
 for (i = 0; i < level->columns * level->rows; i++) {
  if (rand() & 1) {
   level->tiles[i].desc.id = LAND_PLAIN;
   
  }
  else {
   level->tiles[i].desc.id = LAND_MERE;
  }
  
  level->tiles[i].data[DATA_HOLDER] = 0;
  level->tiles[i].data[DATA_HOLDING] = HOLDING_NONE;
 }
 
 // terrain generation
 
 for (i = 0; i < level->columns * level->rows; i++) {
  if (level->tiles[i].desc.id == LAND_PLAIN) {
   y = i / level->columns;
   x = i - (y * level->columns);
   
   if ((rand() % 100) < 35 * (level->rows - y) / level->rows) {
	level->tiles[i].desc.id = LAND_MOUNTAIN;
   }
   else if (rand() % 100 < 25) {
	level->tiles[i].desc.id = LAND_HILL;
   }
  }
 }
 
 // state generation (optional)
 
 for (i = 0; i < states; i++) {
  placestate(i + 1, level);
 }
 
 level->turns = 0;
}

void tickhold(int index, level_t* level) {
 land_u* land;
 
 land = &level->tiles[index];
 
 switch (land->desc.holding) {
 case HOLDING_SEAT:
  level->states[land->desc.holder - 1].stockpile.stocks[STOCK_PRECIOUSMETAL].stock += 1;
  break;
  
 case HOLDING_SETTLED:
  break;
  
 case HOLDING_FARM:
  break;
  
 default:
  break;
 }
}

void tickunit(int index, level_t* level) {
 unit_t* unit = &level->units[index - 1];
 // consume onhand food and supplies; if in battle, commence normal motions
}

void ticktile(int index, level_t* level) {
 land_u* tile;
 
 tile = &level->tiles[index];
 
 switch (tile->desc.id) {
 default:
  break;
 }
 
 if (tile->desc.holding) {
  tickhold(index, level);
 }
 
 if (tile->desc.stationed) {
  tickunit(tile->desc.stationed, level);
 }
}

void tickstate(int index, level_t* level) {
 //tick state markets
 
 
}

// all costs are handled
void ticklevel(level_t* level) {
 int i;
 
 for (i = 0; i < level->columns * level->rows; i++) {
  ticktile(i, level);
 }
 
 for (i = 0; i < MAX_STATES; i++) {
  tickstate(i, level);
 }
 
 // update global market
}

int mapcoordinate(int x, int y) {
 return 0;
}

int calcstateincome(int state, level_t* level) {
 int income = 0;
 
 for (int i = 0; i < level->columns * level->rows; i++) {
  if (level->tiles[i].data[DATA_HOLDER] == state) {
   income += 1;
  }
 }
 
 return income;
}

int calcstateexpense(int state, level_t* level) {
 return 0;
}

void formunit(int tile, int state, level_t* level) {
 int i;
 
 for (i = 0; i < MAX_UNITS; i++) {
  if (!level->units[i].fealty) {
   level->units[i].type = UNIT_FOOTMEN;
   level->units[i].fealty = state;
   level->tiles[tile].data[DATA_STATIONED] = i + 1;
   return;
  }
 }
}

void placestate(int state, level_t* level) {
 int x, y;
 
 //level->states[state].stockpile[STOCK_PRECIOUSMETAL] = 10;
 
redo:
 x = rand() % level->columns;
 y = rand() % level->rows;
 
 if (TRANSLATE(level, x, y).desc.id != LAND_MERE && !TRANSLATE(level, x, y).data[DATA_HOLDER]) {
  TRANSLATE(level, x, y).data[DATA_HOLDING] = HOLDING_SEAT;
  TRANSLATE(level, x, y).data[DATA_HOLDER] = state;
  formunit(y * level->columns + x, state, level);
 }
 else {
  goto redo;
 }
 /*
 for (int i = 0; i < HOLDING_SORCERERS; i++) {
 holdredo:
  x = rand() % level->columns;
  y = rand() % level->rows;
  
  if (TRANSLATE(level, x, y).desc.id != LAND_MERE && !TRANSLATE(level, x, y).data[DATA_HOLDER]) {
   TRANSLATE(level, x, y).data[DATA_HOLDING] = i;
  }
  else {
   goto holdredo;
  }
 }*/
}
/*
void parsepacket(packet_t* pack) {
 // route messages
 for (int i = 0; i < pack->messagecount; i++) {
  extern level_t level;
  message_t* message = &pack->messages[i];
  
  // direct message handling to the level or host
  
  if (pack->messages[i].type == MSG_ADDCLIENT && host.type == HOST_LOCAL) {
   clients[client].address = udppacket->address;
   
   placestate(client + 2, &level);
   
   //send world update
   {
	message_t message;
	
	message.type = MSG_UPDATETILE;
	message.queue = -1;
	
	for (int i = 0; i < level.columns * level.rows; i++) {
	 message.data[0] = i;
	 message.data[1] = level.tiles[i].id;
	 message.data[2] = level.tiles[i].data[DATA_HOLDING];
	 message.data[3] = level.tiles[i].data[DATA_HOLDER];
	 
	 appendclientmessage(&message, client);
	}
	
	message.type = MSG_ADDCLIENT;
	
	message.data[0] = client + 2;
	
	appendclientmessage(&message, client);
	
	readyclient(client);
   }
   
   client++;
  }
  else if (message->type == MSG_ADDCLIENT) {
   
   //mg_player = message->data[0];
  }
  else if (message->type == MSG_UPDATETILE) {
   int index = message->data[0];
   
   printf(" index: %i; id: %i\n", index, message->data[1]);
   
   level.tiles[index].id = message->data[1];
   level.tiles[index].data[DATA_HOLDING] = message->data[2];
   level.tiles[index].data[DATA_HOLDER] = message->data[3];
  }
 }
}
*/

//void 
