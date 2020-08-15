/*
 * puri
 * minimal CLI URL launcher
 */

#include "cmd.h"
#include "ui.h"
#include "unistd.h"

int main(void) {
   if (system("pidof transmission-daemon > /dev/null") != 0) {
      system("transmission-daemon");
      sleep(3);
   }
   cmd_t cmd = init_cmd("transmission-remote -l");
   init_ui(cmd.lines, cmd.outputs);
   drawui();
   drawitems();
   handleinput();
   cleanup();
   return 0;
}
