#include <stdio.h>
#include <stdlib.h>

#include "cmd.h"

cmd_t get_cmd_output(char* cmd_str) {
   char letter;
   int lines, writei;
   cmd_t cmd;
   FILE* pipe;

   pipe = popen(cmd_str, "r");
   fseek(pipe, 0L, SEEK_END);
   cmd.output = malloc(ftell(pipe));

   fseek(pipe, 0L, SEEK_SET);
   while ((letter = fgetc(pipe)) != EOF) {
      if (letter == '\n') ++lines;
      cmd.output[writei++] = letter;
   }
   cmd.lines = lines;
   pclose(pipe);
   return cmd;
}
