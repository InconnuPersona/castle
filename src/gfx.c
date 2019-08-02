#include "gfx.h"
#include "net.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

#include <stdlib.h>
#include <stdio.h>

#define C_RED 0x00ff0000
#define C_GRN 0x0000ff00
#define C_BLU 0x000000ff
#define C_ALP 0xff000000

#define GETRED(Color) ((Color & C_RED) >> (2 * 8))
#define GETGRN(Color) ((Color & C_GRN) >> (1 * 8))
#define GETBLU(Color) ((Color & C_BLU) >> (0 * 8))
#define GETALP(Color) ((Color & C_ALP) >> (3 * 8))

#define RENDER(Texture, Source, Target) SDL_RenderCopy(renderer, Texture, Source, Target)
#define SETCOLORMOD(Texture, Color) SDL_SetTextureColorMod(Texture, GETRED(Color), GETGRN(Color), GETBLU(Color))

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;

SDL_Rect srcrect = { 0, 0, 8, 8 };

void setup_gfx(int w, int h, const char* title) {
 SDL_Init(SDL_INIT_VIDEO);
 IMG_Init(IMG_INIT_PNG);
 TTF_Init();
 SDLNet_Init();
 
 window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
 
 SDLNULLPRONE(window);
 
 renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
 
 SDLNULLPRONE(renderer);
 
 font = TTF_OpenFont("res/timesnewroman.ttf", 16);
 
 SDLNULLPRONE(font);
}

texture_t loadtexture(const char* texture) {
 SDL_Surface* s = IMG_Load(texture);
 
 SDLNULLPRONE(s);
 
 SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
 
 SDLNULLPRONE(t);
 
 SDL_FreeSurface(s);
 
 return t;
}

void renderclear(int color) {
 SDL_SetRenderDrawColor(renderer, GETRED(color), GETGRN(color), GETBLU(color), 0xff);
 SDL_RenderClear(renderer);
}

void renderbox(int x, int y, int w, int h, int color) {
 SDL_Rect rect;
 
 rect.x = x;
 rect.y = y;
 rect.w = w;
 rect.h = h;
 
 SDL_SetRenderDrawColor(renderer, GETRED(color), GETGRN(color), GETBLU(color), 0xff);
 
 SDL_RenderDrawRect(renderer, &rect);
}

void renderflat(int x, int y, int w, int h, int color) {
 SDL_Rect rect;
 
 rect.x = x;
 rect.y = y;
 rect.w = w;
 rect.h = h;
 
 SDL_SetRenderDrawColor(renderer, GETRED(color), GETGRN(color), GETBLU(color), 0xff);
 
 SDL_RenderFillRect(renderer, &rect);
}

void renderlevel(level_t* level, int mx, int my) {
 extern texture_t holdwords[], laywords[];
 
 SDL_Rect rect;
 land_u* tile;
 
 rect.w = rect.h = 8 * SCALE;
 
 renderclear(0);
 
 for (int y = 0; y < level->rows; y++) {
  int skip = y % 2 ? 0 : (rect.w / 2);
  
  for (int x = 0; x < level->columns; x++) {
   rect.x = x * rect.w + skip;
   rect.y = y * rect.h;
   
   tile = &TRANSLATE(level, x, y);
   
   RENDER(landwords[tile->desc.id].texture, &srcrect, &rect);
   
   if (tile->data[DATA_HOLDING] != HOLDING_NONE) {
	RENDER(holdwords[tile->data[DATA_HOLDING]], &srcrect, &rect);
   }
   
   if (tile->data[DATA_HOLDER] > 0) {
	SETCOLORMOD(laywords[LAY_OUTLINE], statepalette[tile->data[DATA_HOLDER] - 1]);
	
	RENDER(laywords[LAY_OUTLINE], &srcrect, &rect);

	SETCOLORMOD(laywords[LAY_OUTLINE], COLOR_BLACK);
   }
   
   if (tile->data[DATA_STATIONED] > 0) {
	RENDER(unitwords[level->units[tile->data[DATA_STATIONED] - 1].type].texture, &srcrect, &rect);
   }
  }
 }
}

void renderpresent() {
 SDL_RenderPresent(renderer);
}

SDL_Texture* getfonttext(const char* string, int color) {
 SDL_Color imagecolor;
 SDL_Surface* surface;
 SDL_Texture* texture;
 
 imagecolor.a = 0xff;
 imagecolor.r = GETRED(color);
 imagecolor.g = GETGRN(color);
 imagecolor.b = GETBLU(color);
 
 surface = TTF_RenderText_Solid(font, string, imagecolor);
 
 texture = SDL_CreateTextureFromSurface(renderer, surface);
 
 SDL_FreeSurface(surface);
 
 return texture;
}

void renderfont(int x, int y, const char* str, int color) {
 SDL_Texture* texture;
 SDL_Rect rect;
 
 texture = getfonttext(str, color);
 
 rect.x = x;
 rect.y = y;
 
 SDL_QueryTexture(texture, 0, 0, &rect.w, &rect.h);
 
 SDL_RenderCopy(renderer, texture, 0, &rect);
 
 SDL_DestroyTexture(texture);
}

void renderboxtext(int x, int y, int w, int h, int boxc, const char* str, int color) {
 SDL_Texture* texture;
 SDL_Rect text;
 
 texture = getfonttext(str, color);
 
 SDL_QueryTexture(texture, 0, 0, &text.w, &text.h);
 
 text.x = (w - text.w) / 2 + x;
 text.y = (h - text.h) / 2 + y;
 
 renderflat(x, y, w, h, boxc);
 
 SDL_RenderCopy(renderer, texture, 0, &text);
 
 SDL_DestroyTexture(texture);
}

