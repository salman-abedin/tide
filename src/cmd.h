#ifndef CMD_H
#define CMD_H

#define LEN(arr) ((int)(sizeof(arr) / sizeof(arr)[0]))

typedef struct {
   int lines;
   char** outputs;
} cmd_t;

cmd_t init_cmd(char* path);

#endif
