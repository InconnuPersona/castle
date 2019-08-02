#pragma once

#include "gfx.h"
#include "net.h"

#include <SDL2/SDL.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define LUAFUNCTION(Name) { #Name, f_##Name, LUAOBJ_FUNCTION }
#define LUANUMBER(Name, Value) { #Name, (void*) Value, LUAOBJ_NUMBER }
#define LUASTRING(Name, String) { #Name, String, LUAOBJ_STRING }
#define LUANULL { NULL, NULL, LUAOBJ_NULL }

#define NEWLUAFUNCTION(Name) int f_##Name(lua_State* L)

#define BEGINLUATABLE(Name) luaobject_t luatable_##Name[] = {
#define ENDLUATABLE LUANULL }

#define TOSTRING(String) #String

typedef enum {
 LUAOBJ_NULL = 0,
 LUAOBJ_NUMBER,
 LUAOBJ_STRING,
 LUAOBJ_FUNCTION
} luaobject_e;

typedef struct {
 char* name;
 void* data;
 luaobject_e type;
} luaobject_t;

char* menutable[] = {
 TOSTRING(MENU_MAIN),
 TOSTRING(MENU_NETWORK),
 TOSTRING(MENU_LOBBY),
 TOSTRING(MENU_OPTIONS),
 
 TOSTRING(MENU_GAME),
 //"MENU_STATUS",
 //"MENU_CHAT",
 //"MENU_TREATY",
 //"MENU_HOLDINGS",
 //"MENU_MARKET",
};

extern int m_select;

void uploadluatable(luaobject_t* table, const char* label, lua_State* L) {
 int index, length;
 
 for (length = 0; table[length].type; length++);
 
 lua_createtable(L, 0, length);
 
 for (index = 0; index < length; index++) {
  switch (table[index].type) {
  case LUAOBJ_NUMBER:
   lua_pushnumber(L, (int) table[index].data);
   lua_setfield(L, -2, table[index].name);
   break;
   
  case LUAOBJ_STRING:
   lua_pushstring(L, table[index].data);
   lua_setfield(L, -2, table[index].name);
   break;
   
  case LUAOBJ_FUNCTION:
   lua_pushcfunction(L, table[index].data);
   lua_setfield(L, -2, table[index].name);
   break;
   
  default:
   break;
  }
 }
 
 lua_setglobal(L, label);
 
 return;
}

void uploadtable(luaobject_t* table, const char* label, lua_State* L) {
 int index, length;
 
 for (length = 0; table[length].type != LUAOBJ_NULL; length++);
 
 lua_createtable(L, 0, length);
 
 for (index = 0; index < length; index++) {
  switch (table[index].type) {
  case LUAOBJ_NUMBER:
   lua_pushnumber(L, (int) table[index].data);
   break;
   
  case LUAOBJ_STRING:
   lua_pushstring(L, table[index].data);
   break;
   
  case LUAOBJ_FUNCTION:
   lua_pushcfunction(L, table[index].data);
   break;
   
  case LUAOBJ_NULL:
   goto uploadtable_end;
   
  default:
   break;
  }
  
  lua_setfield(L, -2, table[index].name);
 }
 
uploadtable_end:
 lua_setglobal(L, label);
 
 return;
}

// key table

extern int getkeyclicked(int);

NEWLUAFUNCTION(clicked) {
 int key = luaL_checknumber(L, 1);
 
 lua_pushboolean(L, getkeyclicked(key));
 
 return 1;
}

// menu table

extern void menuchange(menu_e);

NEWLUAFUNCTION(cueexit) {
 exit(1);
 
 return 0;
}

NEWLUAFUNCTION(change) {
 int menu = luaL_checknumber(L, 1);
 
 menuchange(menu);
 
 return 0;
}

// render table

int luacolor;
int luax, luay;

int f_setcolor(lua_State* L) {
 char r = luaL_checknumber(L, 1);
 char g = luaL_checknumber(L, 2);
 char b = luaL_checknumber(L, 3);
 
 luacolor = (r << 16) | (g << 8) | b;
 
 return 0;
}

int f_setoffset(lua_State* L) {
 luax = luaL_checknumber(L, 1);
 luay = luaL_checknumber(L, 2);
 
 return 0;
}

NEWLUAFUNCTION(chars) {
 int x = luaL_checknumber(L, 1);
 int y = luaL_checknumber(L, 2);
 const char* string = luaL_checkstring(L, 3);
 
 renderfont(x + luax, y + luay, string, luacolor);
 
 return 0;
}

NEWLUAFUNCTION(box) {
 int x, y, w, h;
 
 x = luaL_checknumber(L, 1);
 y = luaL_checknumber(L, 2);
 w = luaL_checknumber(L, 3);
 h = luaL_checknumber(L, 4);
 
 renderbox(x + luax, y + luay, w, h, luacolor);
 
 return 0;
}

// renderbox(1, 16 * row, 8, 16, 0xffff0000);

NEWLUAFUNCTION(flat) {
 int x, y, w, h;
 
 x = luaL_checknumber(L, 1);
 y = luaL_checknumber(L, 2);
 w = luaL_checknumber(L, 3);
 h = luaL_checknumber(L, 4);
 
 renderflat(x, y, w, h, luacolor);
 
 return 0;
}

// table structures

BEGINLUATABLE(key)
 LUANUMBER(UP, SDLK_UP),
 LUANUMBER(DOWN, SDLK_DOWN),
 LUANUMBER(ENTER, SDLK_RETURN),
 
 LUAFUNCTION(clicked),
ENDLUATABLE;

BEGINLUATABLE(menu)
 LUANUMBER(MAIN, MENU_MAIN),
 LUANUMBER(GAME, MENU_GAME),
 LUANUMBER(NETWORK, MENU_NETWORK),
 LUANUMBER(LOBBY, MENU_LOBBY),
 
 LUAFUNCTION(cueexit),
 LUAFUNCTION(change),
ENDLUATABLE;

BEGINLUATABLE(render)
 LUANUMBER(WIDTH, WINDOW_WIDTH),
 LUANUMBER(HEIGHT, WINDOW_HEIGHT),
 
 LUAFUNCTION(box),
 LUAFUNCTION(flat),
 LUAFUNCTION(setcolor),
 LUAFUNCTION(setoffset),
 LUAFUNCTION(chars),
ENDLUATABLE;

void loadluaglobals(lua_State* L) {
 uploadluatable(luatable_key, "key", L);
 uploadluatable(luatable_menu, "menu", L);
 uploadluatable(luatable_render, "render", L);
}
