#include "gfx.h"
#include "net.h"

#if USE_LUAMENU

#include <stdlib.h>
#include <memory.h>

#include "luabind.h"

typedef int (*luafunction_f)(lua_State*);

menu_e menu = MENU_MAIN;

lua_State* L;

extern void setlisten(int);

void loadmenu(char* string) {
 char buffer[32];
 
 memset(buffer, 0, 32);
 
 sprintf(buffer, "res/menu/%s.lua", string);
 
 luaL_dofile(L, buffer);
 
 lua_setglobal(L, string);
 
 lua_settop(L, 0);
}

void enablemenus() {
 int i;
 
 L = luaL_newstate();
 
 luaL_openlibs(L);
 
 loadluaglobals(L);
 
 // menu scripts
 for (i = 0; i < sizeof(menutable) / sizeof(char*); i++) {
  loadmenu(menutable[i]);
 }
}

void disablemenus() {
 lua_close(L);
}

void menuchange(menu_e nmenu) {
 setlisten(0);
 
 menu = nmenu;
}

void menuchar(int character) {
}

void menumouse(int x, int y, int state) {
}

void menutick() {
 lua_getglobal(L, menutable[menu]);
 lua_getfield(L, -1, "tick");
 lua_call(L, 0, 0);
}

void menurender() {
 lua_getglobal(L, menutable[menu]);
 lua_getfield(L, -1, "render");
 lua_call(L, 0, 0);
}

#endif
