#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"

void _verify_running() {
   if (system("pidof transmission-daemon > /dev/null 2>&1") != 0)
      system("transmission-daemon");
}

cmd_t init_cmd(char* cmd_str) {
   int lines, capacity, i;
   char line[1024];
   FILE* pipe;
   cmd_t cmd;

   capacity = 50;
   cmd.outputs = calloc(capacity, sizeof(char*));
   for (i = 0; i < capacity; ++i)
      cmd.outputs[i] = calloc(sizeof(line), sizeof(char));

   _verify_running();
   pipe = popen(cmd_str, "r");

   lines = 0;
   while (fgets(line, sizeof line, pipe)) {
      ++lines;
      if (lines == 1) continue;
      line[strcspn(line, "\n")] = 0;  // Remove newlines
      strcpy(cmd.outputs[lines - 2], line);
      if (lines - 2 == capacity) {
         capacity *= 2;
         cmd.outputs = realloc(cmd.outputs, sizeof(char*) * capacity);
         for (i = 0; i < capacity; ++i)
            cmd.outputs[i] = calloc(sizeof(line), sizeof(char));
      }
   }
   cmd.lines = lines - 2;
   pclose(pipe);

   return cmd;
}
