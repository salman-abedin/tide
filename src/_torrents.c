#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../config.h"
#include "_torrents.h"

void _verify_running() {
   char cmd_str[1024];

   sprintf(cmd_str, "pidof transmission-daemon > /dev/null 2>&1");

   if (system(cmd_str) != 0) {
      sprintf(cmd_str, "transmission-daemon > /dev/null 2>&1");
      system(cmd_str);
      sleep(2);
   }
}

Torrents init_cmd(char* cmd_str) {
   int lines, capacity;
   char line[1024];
   FILE* pipe;
   Torrents torrents;

   capacity = 50;
   torrents.list = calloc(capacity, sizeof(char*));

   _verify_running();
   pipe = popen(cmd_str, "r");

   lines = 0;
   while (fgets(line, sizeof line, pipe)) {
      line[strcspn(line, "\n")] = 0;
      torrents.list[lines] = calloc(sizeof line, sizeof(char));
      strcpy(torrents.list[lines++], line);

      if (lines == capacity) {
         capacity *= 2;
         torrents.list = realloc(torrents.list, sizeof(char*) * capacity);
      }
   }
   torrents.count = lines;
   pclose(pipe);
   return torrents;
}
