/*
 * puri
 * minimal CLI URL launcher
 */

#include "cmd.h"
#include "ui.h"
#include "stdio.h"

int main(void) {
   cmd_t cmd = get_cmd_output("ls");
   printf("%s", cmd.output);
   /* init(urls.count, urls.links); */
   /* drawui(); */
   /* drawitems(); */
   /* handleinput(); */
   /* cleanup(); */
   return 0;
}
