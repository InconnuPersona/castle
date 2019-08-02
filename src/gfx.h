#pragma once

#include "land.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define SCALE 4
#define FACTOR

#define TILE_WIDTH (8 * SCALE)

#define MAX_EVENTCHARMS 4
#define CURRENTTIME -2

typedef enum {
 COLOR_TRANSPARENT = 0x00000000,
 COLOR_WHITE = 0xffffffff,
 COLOR_BLACK = 0xff000000,
 COLOR_GRAY = 0xff808080,
 COLOR_RED = 0xffff0000,
 COLOR_HALFRED = 0xff800000,
 COLOR_GREEN = 0xff00ff00,
 COLOR_HALFGREEN = 0xff008000,
 COLOR_BLUE = 0xff0000ff,
 COLOR_HALFBLUE = 0xff000080,
 COLOR_YELLOW = COLOR_RED | COLOR_GREEN,
 COLOR_CYAN = COLOR_GREEN | COLOR_BLUE,
 COLOR_MAGENTA = COLOR_RED | COLOR_BLUE,
} color_e;

typedef struct {
 float minx, miny;
 float maxx, maxy;
} quad_t, aabb_t;

typedef struct {
} sprite_t;

typedef struct {
 char* text;
} eventcharm_t;

typedef struct {
 char* title;
 char* description;
 eventcharm_t charms[MAX_EVENTCHARMS];
} popup_t;

typedef enum {
 TIMER_STEPPED,
 TIMER_FALSELAPSE,
 TIMER_PATTERN,
} timer_e;

typedef struct {
 timer_e type;
 int set; // bool
 float start, end;
 float value;
} watch_t;

typedef void* texture_t;

void pollevents();
void enablemenu();
void disablemenus();
void menutick();
void menurender();
void tickkeys();

void settimer(watch_t* watch, timer_e type, float start, float value);
float readtimer(watch_t* watch);
void seedrandom(int seed);
void seedrandomtime();
void delaythread(float seconds);

void setup_gfx(int w, int h, const char* title);

texture_t loadtexture(const char* texture);

void renderbox(int x, int y, int w, int h, int color);
void renderflat(int x, int y, int w, int h, int color);
void renderfont(int x, int y, const char* str, int color);
void renderboxtext(int x, int y, int w, int h, int boxcolor, const char* str, int color);

void renderpresent();
