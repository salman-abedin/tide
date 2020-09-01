#ifndef TORRENTS_H
#define TORRENTS_H

typedef struct {
   int count;
   char** list;
} Torrents;

Torrents init_cmd(char* path);

#endif
