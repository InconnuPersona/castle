// MENU_GAME

#define AGLANCEOFFSET WNDWDIV(4) * 3 + 8
#define PLAYERSTATE level.states[mg_player]
#define QUARTERWND (WINDOW_WIDTH / 4)
#define WNDWDIV(Number) (WINDOW_WIDTH / Number)
#define TILEFIELD(Tile, Field) level.tiles[Tile].desc.Field
#define MARKETGOOD(Market, Good) Market.stocks[Good].stock
#define RTILINE(Row, ...) { sprintf(buffer, __VA_ARGS__); renderfont(AGLANCEOFFSET, (WINDOW_HEIGHT / 2) + ((Row + 3) * 16), buffer, COLOR_WHITE); }

enum {
 GAME_MENU,
 GAME_OVERVIEW,
 
 GAME_STATUS, // state status; treaties, modifiers, etc.
 GAME_BUILD, //
 GAME_MARKET,
 GAME_CHAT,
 
 GAME_ALL
} gamemenu = GAME_OVERVIEW;

int mg_sx = 0, mg_sy = 0;
int mg_select = -1, mg_player = -1;

watch_t mg_timer;

void mg_line(int line, const char* format, ...) {
 renderfont(AGLANCEOFFSET, 16 * line, format, COLOR_WHITE);
}

void mg_button(int line, const char* format, ...) {
 renderboxtext(AGLANCEOFFSET, 16 * line, 100, 16, COLOR_BLACK, format, COLOR_WHITE);
}

int mg_collide(int line, int x, int y) {
 return (x >= AGLANCEOFFSET) && (x <= AGLANCEOFFSET + 100) && (y >= 16 * line) && (y <= 16 * (line + 1));
}

void mg_choice(int x, int y) {
 if (mg_collide(8, x, y)) {
  gamemenu = GAME_STATUS;
 }
 else if (mg_collide(10, x, y)) {
  gamemenu = GAME_BUILD;
 }
 else if (mg_collide(12, x, y)) {
  gamemenu = GAME_MARKET;
 }
 else if (mg_collide(14, x, y)) {
  gamemenu = GAME_CHAT;
 }
 else if (mg_collide(16, x, y)) {
  menuchange(MENU_MAIN);
 }
 else {
  // handle map click
  
  int index;
  int tx, ty;
  
  ty = y / TILE_WIDTH;
  
  tx = x - (ty % 2 ? 0 : (TILE_WIDTH / 2));
  
  tx /= TILE_WIDTH;
  
  if (ty < 0 || ty > level.rows || tx < 0 || tx > level.columns) {
   mg_select = -1;
   return;
  }
  
  index = ty * level.columns + tx;
  
  mg_select = index;
 }
}

void mg_marketchoice(int x, int y) {
 if (mg_collide(16, x, y)) {
  gamemenu = GAME_OVERVIEW;
 }
}

void mg_handlechoice(int x, int y) {
 switch (gamemenu) {
 case GAME_OVERVIEW:
  mg_choice(x, y);
  break;
  
 case GAME_MARKET:
  mg_marketchoice(x, y);
  break;
  
 default:
  break;
 }
}

void mg_tick() {
 
 if (!mg_timer.set) {
  settimer(&mg_timer, TIMER_FALSELAPSE, CURRENTTIME, 3); // TODO: make a true lapse
 }
 
 if (readtimer(&mg_timer) >= 0) { // T-10 seconds vs. T+10 seconds
  setturn(&level);
 }
 
 if (mg_player < 0) {
  mg_player = 1; // gethostid(); send player id with addclient message
	
  PLAYERSTATE.ai = 0;
 }
}

void mg_rendertileinfo() {
 char buffer[32];
 
 if (mg_select > -1) {
  RTILINE(0, "Tile info:");
  RTILINE(1, "ID: %i", TILEFIELD(mg_select, id));
  RTILINE(2, "UNDEF: %i", TILEFIELD(mg_select, undefined));
  RTILINE(3, "CLAIM: %i", TILEFIELD(mg_select, claimant));
  RTILINE(4, "OWNER: %i", TILEFIELD(mg_select, holder));
  RTILINE(5, "HOLD: %i", TILEFIELD(mg_select, holding));
  RTILINE(6, "UNIT: %i", TILEFIELD(mg_select, stationed));
  RTILINE(7, "DEV: %i", TILEFIELD(mg_select, development));
  RTILINE(8, "RES: %i", TILEFIELD(mg_select, resource));
 }
}

void mg_renderoverview() {
 char buffer[32];
 
 memset(buffer, 0, sizeof(buffer));
 
 mg_line(0, "Our glorious state");
 
 sprintf(buffer, "Turn %i", level.turns);
 mg_line(1, buffer);
 
 mg_line(3, "Treasury:");
 
 sprintf(buffer, " %i c.u.", MARKETGOOD(PLAYERSTATE.stockpile, STOCK_PRECIOUSMETAL));
 mg_line(4, buffer);
 
 mg_line(5, "Income:");
 
 sprintf(buffer, " %i c.u.", calcstateincome(mg_player, &level));
 mg_line(6, buffer);
 
 mg_button(8, "Status");
 mg_button(10, "Build");
 mg_button(12, "Market");
 mg_button(14, "Chat");
 mg_button(16, "Resign");
 
 mg_rendertileinfo();
}

void mg_rendermarket() {
 char buffer[32];
 
 memset(buffer, 0, sizeof(buffer));
 
 // state market
 {
  mg_line(0, "State stockpiles:");
  
  sprintf(buffer, " PRCMETAL: %i", MARKETGOOD(PLAYERSTATE.stockpile, STOCK_PRECIOUSMETAL));
  mg_line(1, buffer);
  sprintf(buffer, " ORGANICS: %i", MARKETGOOD(PLAYERSTATE.stockpile, STOCK_ORGANICS));
  mg_line(2, buffer);
  sprintf(buffer, " RESEARCH: %i", MARKETGOOD(PLAYERSTATE.stockpile, STOCK_RESEARCH));
  mg_line(3, buffer);
  sprintf(buffer, " SIDEARMS: %i", MARKETGOOD(PLAYERSTATE.stockpile, STOCK_SIDEARMS));
  mg_line(4, buffer);
  sprintf(buffer, " FIREARMS: %i", MARKETGOOD(PLAYERSTATE.stockpile, STOCK_FIREARMS));
  mg_line(5, buffer);
  sprintf(buffer, " CLOTHING: %i", MARKETGOOD(PLAYERSTATE.stockpile, STOCK_CLOTHING));
  mg_line(6, buffer);
  sprintf(buffer, " FRGMETAL: %i", MARKETGOOD(PLAYERSTATE.stockpile, STOCK_FORGEMETAL));
  mg_line(7, buffer);
  sprintf(buffer, " WRITING: %i", MARKETGOOD(PLAYERSTATE.stockpile, STOCK_WRITING));
  mg_line(8, buffer);
  sprintf(buffer, " MATERIAL: %i", MARKETGOOD(PLAYERSTATE.stockpile, STOCK_MATERIAL));
  mg_line(9, buffer);
  
  mg_button(16, "Return");
 }
 
 // local market
 {
  
 }
 
 // global market
 {
  
 }
}

void mg_render_aglance() {
 renderflat(WNDWDIV(4) * 3, 0, WNDWDIV(4), WINDOW_HEIGHT, 0x808080);
 
 switch (gamemenu) {
 case GAME_OVERVIEW:
  mg_renderoverview();
  break;
  
 case GAME_MARKET:
  mg_rendermarket();
  break;
  
 default:
  break;
 }
}
