/*
 * puri
 * minimal CLI URL launcher
 */

#include "stdlib.h"
#include "ui.h"
#include "unistd.h"

int main(void) {
   if (system("pidof transmission-daemon > /dev/null") != 0) {
      system("transmission-daemon");
      sleep(3);
   }
   init_ui();
   drawui();
   drawitems();
   handleinput();
   cleanup();
   return 0;
}
