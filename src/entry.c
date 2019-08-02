#include "gfx.h"
#include "net.h"

#undef main

level_t level;

extern void enablemenus();

int main(int argc, char** argv) {
 watch_t timer;
 float delay = 1 / 30.f;
 
 setup_gfx(640, 480, "Castles domain");
 
 setup_lands();
 
 enablemenus();
 
 seedrandomtime();
 
 settimer(&timer, TIMER_FALSELAPSE, 0, delay);
 
 while (1) {
  pollevents();
  
  handlequeues();
  
  if (readtimer(&timer) >= 0) {
   tickkeys();
   
   menutick();
   
   updatequeues();
   
   menurender();
   
   renderpresent();
  }
  
  delaythread(delay);
 }
 
 return 0;
}
