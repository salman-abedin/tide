/*
 * puri
 * minimal CLI URL launcher
 */

#include "cmd.h"
#include "stdio.h"
#include "ui.h"

int main(void) {
   int cmd_return = system("pidof transmission-daemon > /dev/null");
   printf("%d", cmd_return);
   /* if (system("transmission-daemon")) */
   /* printf("Running"); */
   /* else */
   /* printf("Not running"); */

   /* cmd_t cmd = init_cmd("transmission-remote -l"); */
   /* printf("%s", cmd.output); */
   /* init(urls.count, urls.links); */
   /* drawui(); */
   /* drawitems(); */
   /* handleinput(); */
   /* cleanup(); */
   return 0;
}
