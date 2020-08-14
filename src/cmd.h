#ifndef CMD_H
#define CMD_H

typedef struct {
   int lines;
   char* output;
} cmd_t;

cmd_t get_cmd_output(char* path);

#endif
