#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"

cmd_t init_cmd(char* cmd_str) {
   int lines, capacity, i;
   char line[1024];
   FILE* pipe;
   cmd_t cmd;

   capacity = 50;
   cmd.outputs = calloc(capacity, sizeof(char*));
   for (i = 0; i < capacity; ++i)
      cmd.outputs[i] = calloc(sizeof(line), sizeof(char));

   pipe = popen(cmd_str, "r");

   lines = 0;
   while (fgets(line, 1024, pipe)) {
      ++lines;
      if (lines == 1) continue;
      strcpy(cmd.outputs[lines - 2], line);
      if (lines - 2 == capacity) {
         capacity *= 2;
         cmd.outputs = realloc(cmd.outputs, sizeof(char*) * capacity);
         for (i = 0; i < capacity; ++i)
            cmd.outputs[i] = calloc(sizeof(line), sizeof(char));
      }
   }
   pclose(pipe);
   cmd.lines = lines - 2;
   return cmd;
}
