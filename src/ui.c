#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"

int mark, start, end, width, height, wwidth, wheight, count, i, j;
char** items;
WINDOW *win, *header, *footer;

void init_ui(int in_count, char** in_items) {
   items = in_items;
   count = in_count;
   initscr();
   cbreak();
   noecho();
   curs_set(0);
   getmaxyx(stdscr, height, width);
   wwidth = width;
   wheight = height - 4;
   end = count > wheight - 2 ? wheight - 2 : count;
}

void drawui() {
   mvprintw(0, (width - strlen(HEADER)) / 2, HEADER);
   mvprintw(height - 1, (width - strlen(FOOTER)) / 2, FOOTER);
   win = newwin(wheight, wwidth, 2, (width - wwidth) / 2);
   refresh();
}

void drawitems() {
   werase(win);
   box(win, 0, 0);
   for (i = 1, j = start; j < end; ++i, ++j) {
      if (i - 1 == mark) wattron(win, A_REVERSE);
      mvwaddnstr(win, i, 1, items[j], wwidth - 2);
      wattroff(win, A_REVERSE);
   }
}

void handleinput() {
   int key;
   char* cmd;
   char* cmdhead = "$BROWSER";
   while ((key = wgetch(win)) != 'h') {
      if (key == 'j') {
         if (mark < wheight - 3 && mark < end - 1) {
            ++mark;
         } else if (end < count) {
            ++end;
            ++start;
         } else {
            end = count > wheight - 2 ? wheight - 2 : count;
            start = mark = 0;
         }
         drawitems();
      } else if (key == 'k') {
         if (mark == 0 && start > 0) {
            --end;
            --start;
         } else {
            if (mark > 0) {
               --mark;
            } else {
               end = count;
               mark = count > wheight - 2 ? wheight - 3 : count - 1;
               start = count > wheight - 2 ? count - wheight + 2 : 0;
            }
         }
         drawitems();
      } else if (key == 'l') {
         cmd = calloc(strlen(cmdhead) + strlen(items[mark]) + 2, (sizeof *cmd));
         sprintf(cmd, "%s %s", cmdhead, items[mark]);
         system(cmd);
      }
   }
}

void cleanup() {
   for (i = 0; i < count; ++i) free(items[i]);
   delwin(win);
   free(items);
   endwin();
}
