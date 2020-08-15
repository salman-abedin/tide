#include <stdio.h>
#include <stdlib.h>

#include "cmd.h"

cmd_t init_cmd(char* cmd_str) {
   int lines, letters, capacity, writei;
   char letter;
   FILE* pipe;
   cmd_t cmd;

   lines = letters = writei = 0;
   capacity = 1024;

   cmd.output = calloc(capacity, sizeof(char));
   pipe = popen(cmd_str, "r");

   while ((letter = fgetc(pipe)) != EOF) {
      if (letter == '\n') ++lines;
      if (++letters == capacity) {
         capacity *= 2;
         cmd.output = realloc(cmd.output, sizeof(char) * capacity);
      }
      cmd.output[writei++] = letter;
   }
   pclose(pipe);

   cmd.lines = lines;
   return cmd;
}
