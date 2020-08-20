#ifndef UI_H
#define UI_H

#define HEADER                                                                \
   "    ID   Done       Have  ETA           Up    Down  Ratio  Status       " \
   "Name"

#define RUNNING_PAIR 0
#define STOPPED_PAIR 1
#define COMPLETED_PAIR 2

void init_ui(void);
void draw_ui(void);
void handle_input(void);
void housekeep(void);

#endif
