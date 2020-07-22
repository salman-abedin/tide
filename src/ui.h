#ifndef UI_H
#define UI_H

#define HEADER "tide: Tiny Transmission Client"
#define FOOTER "h:Stop   j:Down   k:Up   l:Start   d:Delete   q:Quit"

void init_ui();
void drawui();
void handleinput();
void cleanup();

#endif
