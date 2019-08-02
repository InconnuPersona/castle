#include "gfx.h"
#include "net.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

void calcmarketrates(market_t* market) {
 
}

//void setstate(state_t* state) {
 
 
 
 //calcmarketrates(state->market);
 //calcmarketrates(state->stockpile);
//}

void setturn(level_t* level) {
 level->turns++;
 
 ticklevel(level);
 
}

void routepacket(packet_t* packet) {
 // route messages
 for (int i = 0; i < packet->messagecount; i++) {
  message_t* message = &packet->messages[i];
  
  // direct message handling to the level or host
  
 }
}

void settimer(watch_t* watch, timer_e type, float start, float value) {
 
 watch->type = type;
 if (start != CURRENTTIME) {
  watch->start = watch->end = start;
 }
 else {
  watch->start = watch->end = SDL_GetTicks() / 1000.f;
 }
 watch->value = value;
 watch->set = 1;
 
 return;
}

float readtimer(watch_t* watch) {
 float value, other;
 
 switch (watch->type) {
 case TIMER_FALSELAPSE:
  value = SDL_GetTicks() / 1000.f;
  
  other = value - (watch->end + watch->value);
  
  if (other >= 0) {
   watch->end = value;
  }
  
  return other;
  
 default:
  break;
 }
 
 return 0;
}

void seedrandom(int seed) {
 srand(seed);
}

void seedrandomtime() {
 seedrandom(time(NULL));
}

void delaythread(float seconds) {
 fflush(stdout);
 
 SDL_Delay(seconds * 1000.f);
}

int reword(int word, object_e object, vector_e vector) {
 
 return 0;
}
