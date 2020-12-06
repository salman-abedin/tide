#ifndef UI_H
#define UI_H

#define HEADER                                                                 \
  "    ID   Done       Have  ETA           Up    Down  Ratio  Status       "   \
  "Name"

enum Color_Pairs { Pair_Running = 1, Pair_Stopped, Pair_Completed };

void init_ui(void);
void draw_ui(void);
void* handle_input();
void* get_torrents();
void housekeep(void);

#endif
