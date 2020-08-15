/*
 * puri
 * minimal CLI URL launcher
 */

#include "ui.h"

int main(void) {
   init_ui();
   drawui();
   handleinput();
   cleanup();
   return 0;
}
