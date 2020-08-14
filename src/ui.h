#ifndef UI_H
#define UI_H

#define HEADER "puri: Puny URL Launcher"
#define FOOTER "h:Quit   j:Down   k:Up   l:launch"

void init(int in_count, char** in_items);
void drawui();
void drawitems();
void handleinput();
void cleanup();

#endif
