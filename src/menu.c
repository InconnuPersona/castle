#include "gfx.h"
#include "net.h"

#if !USE_LUAMENU

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>

extern level_t level;

extern int getkeyclicked(int);
extern void menuchange(menu_e menu);
extern void setlisten(int state);

menu_e menu = MENU_MAIN;

#if 1

// MENU

#define M_MOVES \
 if (getkeyclicked(SDLK_UP)) { m_select--; } \
 if (getkeyclicked(SDLK_DOWN)) { m_select++; }

#define M_CHOSEN \
 getkeyclicked(SDLK_RETURN)

#define M_LIMIT(Bound) \
 if (m_select > Bound) { m_select = 0; } \
 else if (m_select < 0) { m_select = Bound; }

#define M_APPEND(Output, String1, String2) \
 strcpy(Output, String1); \
 strcat(Output, String2);

#define M_LINE(Line, String) \
 renderfont(8, 16 * Line, String, COLOR_WHITE);

#define M_RELINE(Line, Select, String) \
 M_LINE(Line, String); \
 if (m_select == Select) { \
  renderbox(1, 16 * Line, 8, 16, 0xffff0000); \
 }

int m_select = 0;

// MAIN_MENU

void mm_tick() {
 M_MOVES;
 M_LIMIT(4);
 
 if (M_CHOSEN) {
  switch (m_select) {
  case 0:
   menuchange(MENU_GAME);
   
   init_level(&level, 8, 8);
   
   generate(&level, 1);
   break;
   
  case 1:
   menuchange(MENU_NETWORK);
   break;
   
  case 2:
   break;
   
  case 3:
   break;
   
  case 4:
   exit(0);
   break;
   
  default:
   break;
  }
 }
}

void mm_render() {
 renderflat(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, COLOR_BLACK);
 
 M_LINE(1, "Main menu - welcome to castles!");
 
 M_RELINE(3, 0, "Start");
 M_RELINE(4, 1, "Network");
 M_RELINE(5, 2, "Options");
 M_RELINE(6, 3, "Adventure (secret)");
 M_RELINE(7, 4, "Quit");
}

// MENU_NETWORK

char mn_address[16] = { 0 }, mn_port[16] = { 0 };

packet_t mn_packet;

void mn_menuchar(int character) {
 if (m_select == 0) {
  if (((character >= '0') && (character <= '9')) || (character == '.')) {
   mn_address[strlen(mn_address)] = (char) character;
  }
  else if (character == SDLK_BACKSPACE) {
   mn_address[strlen(mn_address) - 1] = '\0';
  }
 }
 else if (m_select == 1) {
  if (((character >= '0') && (character <= '9')) || (character == '.')) {
   mn_port[strlen(mn_port)] = (char) character;
  }
  else if (character == SDLK_BACKSPACE) {
   mn_port[strlen(mn_port) - 1] = '\0';
  }
 }
}

void mn_tick() {
 message_t message;
 
 M_MOVES;
 M_LIMIT(4);
 
 if (M_CHOSEN) {
  switch (m_select) {
  case 0:
   m_select++;
   break;
   
  case 1:
   m_select++;
   break;
   
  case 2:
   joinhost(mn_address, atoi(mn_port), 3652);
   
   message.type = MSG_ADDCLIENT;
   message.queue = givequeue("unknownhost");
   
   appendmessage(&message, givequeue("host"));
   
   readyqueue(givequeue("host"));
   
   init_level(&level, 8, 8);
   
   menuchange(MENU_GAME);
   break;
   
  case 3:
   openhost(atoi(mn_port));
   
   init_level(&level, 8, 8);
   
   generate(&level, 1);
   
   menuchange(MENU_GAME);
   break;
   
  case 4:
   menuchange(MENU_MAIN);
   break;
   
  default:
   break;
  }
 }
}

void mn_render() {
 char t_addr[32], t_port[32];
 
 t_addr[31] = '\0';
 t_port[31] = '\0';
 
 M_APPEND(t_addr, "Address: ", mn_address);
 M_APPEND(t_port, "Port: ", mn_port);
 
 renderflat(1, 1, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1, COLOR_BLACK);
 
 M_LINE(1, "Network menu - enter server address and port");
 
 M_RELINE(3, 0, t_addr);
 M_RELINE(4, 1, t_port);
 
 M_RELINE(6, 2, "Join server");
 M_RELINE(7, 3, "Start server");
 M_RELINE(8, 4, "Return");
}

#endif

#include "game.h"
#include "lobby.h"

// menu functions

void enablemenus() {
}

void disablemenus() {
}

void menuchange(menu_e nmenu) {
 m_select = 0;
 
 setlisten(0);
 
 switch (nmenu) {
 case MENU_NETWORK:
  setlisten(1);
  break;
  
 default:
  break;
 }
 
 menu = nmenu;
}

void menuchar(int character) {
 switch (menu) {
 case MENU_NETWORK:
  mn_menuchar(character);
  break;
  
 default:
  setlisten(0);
  break;
 }
}

void menumouse(int x, int y, int state) {
 static int mousedown = 0;
 
 if (!state) {
  mousedown = 0;
  return;
 }
 else if (mousedown) {
  // handle screenpans later
  return;
 }
 
 if (!mousedown) {
  mousedown = 1;
 }
 
 switch (menu) {
 case MENU_GAME:
  mg_handlechoice(x, y);
  break;
  
 default:
  break;
 }
}

void menutick() {
 switch (menu) {
 case MENU_MAIN:
  mm_tick();
  break;
  
 case MENU_NETWORK:
  mn_tick();
  break;
  
 case MENU_GAME:
  mg_tick();
  break;
  
 default:
  break;
 }
}

void menurender() {
 switch (menu) {
 case MENU_MAIN:
  mm_render();
  break;
  
 case MENU_NETWORK:
  mn_render();
  break;
  
 case MENU_GAME:
  renderlevel(&level, 0, 0);
  
  mg_render_aglance();
  break;
  
 default:
  break;
 }
}

#endif
