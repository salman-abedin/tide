#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../config.h"
#include "cmd.h"

void _verify_running() {
   char server_prefix[256] = {0};
   char cmd_str[1024];

   if (REMOTE_USE == 1)
      sprintf(server_prefix, "ssh -p %s %s@%s", REMOTE_PORT, REMOTE_USER,
              REMOTE_IP);
   sprintf(cmd_str, "%s %s", server_prefix,
           "pidof transmission-daemon > /dev/null 2>&1");

   if (system(cmd_str) != 0) {
      sprintf(cmd_str, "%s %s", server_prefix,
              "transmission-daemon > /dev/null 2>&1");
      system(cmd_str);
   }
}

cmd_t init_cmd(char* cmd_str) {
   int lines, capacity, i;
   char line[1024];
   FILE* pipe;
   cmd_t cmd;

   capacity = 50;
   cmd.outputs = calloc(capacity, sizeof(char*));
   for (i = 0; i < capacity; ++i)
      cmd.outputs[i] = calloc(sizeof line, sizeof(char));

   _verify_running();
   pipe = popen(cmd_str, "r");

   lines = 0;
   while (fgets(line, sizeof line, pipe)) {
      ++lines;
      if (lines == 1) continue;
      line[strcspn(line, "\n")] = 0;
      strcpy(cmd.outputs[lines - 2], line);
      if (lines - 2 == capacity) {
         capacity *= 2;
         cmd.outputs = realloc(cmd.outputs, sizeof(char*) * capacity);
         for (i = 0; i < capacity; ++i)
            cmd.outputs[i] = calloc(sizeof line, sizeof(char));
      }
   }
   cmd.lines = lines - 2;
   pclose(pipe);

   return cmd;
}
