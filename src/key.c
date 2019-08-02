#include <stdlib.h>

#include <SDL2/SDL.h>

#define MAX_KEYS 128

extern void menuchar(int);
extern void menumouse(int, int, int);

typedef struct {
 int code;
 int state, clicked;
 int presses, absorbs;
} ckey_t;

int key_listen = 0; // boolean

ckey_t keys[MAX_KEYS] = { 0 };

int addkey(int code) {
 int last = -1;
 
 for (int i = 0; i < MAX_KEYS; i++) {
  if (keys[i].code == 0) {
   last = i;
  }
  else if (keys[i].code == code) {
   return i;
  }
 }
 
 if (last > -1) {
  keys[last].code = code;
 }
 
 return last;
}

int getkey(int key) {
 for (int i = 0; i < MAX_KEYS; i++) {
  if (keys[i].code == key) {
   return i;
  }
 }
 
 return -1;
}

void setkey(int code, int state) {
 int i = getkey(code);
 
 if (i < 0) {
  i = addkey(code);
 }
 
 if (i > 0) {
  keys[i].state = state;
  
  if (state) {
   keys[i].presses++;
  }
 }
}

int getkeyclicked(int key) {
 int i = getkey(key);
 
 if (i > -1) {
  return keys[i].clicked;
 }
 else {
  return 0;
 }
}

void clearkeys() {
 for (int i = 0; i < MAX_KEYS; i++) {
  keys[i].state = 0;
 }
}

void setlisten(int state) {
 key_listen = state;
}

void tickkeys() {
 for (int i = 0; i < MAX_KEYS; i++) {
  if (keys[i].absorbs < keys[i].presses) {
   keys[i].absorbs++;
   keys[i].clicked = 1;
  }
  else {
   keys[i].clicked = 0;
  }
 }
}

void pollevents() {
 SDL_Event e;
 
 while (SDL_PollEvent(&e)) {
  switch (e.type) {
  case SDL_QUIT:
   exit(0);
   break;
   
  case SDL_MOUSEBUTTONUP:
  case SDL_MOUSEBUTTONDOWN:
   if (e.button.button == SDL_BUTTON_LEFT) {
	menumouse(e.button.x, e.button.y, e.button.state);
   }
   break;
   
  case SDL_MOUSEMOTION:
   // currently ignore this; menumouse(x, y, -1, -1);
   break;
   
  case SDL_MOUSEWHEEL:
   break;
   
  case SDL_KEYDOWN:
  case SDL_KEYUP:
   if (key_listen && e.key.state) {
	menuchar(e.key.keysym.sym);
   }
   
   setkey(e.key.keysym.sym, e.key.state);
   break;
   
  default:
   break;
  }
 }
}
