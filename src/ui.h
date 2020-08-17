#ifndef UI_H
#define UI_H

#define HEADER "tide: Tiny Transmission Client"
#define FOOTER "h:Stop   j:Down   k:Up   l:Start   d:Delete   q:Quit"

#define RUNNING_PAIR 1
#define STOPPED_PAIR 2
#define COMPLETED_PAIR 3

void init_ui(void);
void draw_ui(void);
void handle_input(void);
void housekeep(void);

#endif
