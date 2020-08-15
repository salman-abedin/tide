#ifndef CMD_H
#define CMD_H

typedef struct {
   int lines;
   char* output;
} cmd_t;

cmd_t init_cmd(char* path);

#endif
