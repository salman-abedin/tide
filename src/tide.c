/*
 * tide
 * minimal Transmission CLI client
 */

#include "ui.h"

int main(void) {
   init_ui();
   draw_ui();
   handle_input();
   housekeep();
   return 0;
}
