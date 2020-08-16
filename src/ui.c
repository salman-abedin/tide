#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"
#include "ui.h"

int mark, start, end, width, height, wwidth, wheight, count, i, j;
char** items;
WINDOW *win, *header, *footer;

char logs[1034];

void init_ui() {
   initscr();
   cbreak();
   noecho();
   curs_set(0);

   /* signal(SIGWINCH, resize_term(height, width)); */
   signal(SIGWINCH, drawui);

   start_color();
   use_default_colors();
   init_pair(RUNNING_PAIR, COLOR_YELLOW, -1);
   init_pair(STOPPED_PAIR, COLOR_RED, -1);
   init_pair(COMPLETED_PAIR, COLOR_GREEN, -1);

   mark = start = 0;
}

void drawui() {
   resize_term(height, width);
   getmaxyx(stdscr, height, width);

   wwidth = width;
   wheight = height - 2;

   sprintf(logs, "ns %d", width);
   system(logs);

   /* attron(A_DIM); */
   mvprintw(0, (width - strlen(HEADER)) / 2, HEADER);
   mvprintw(height - 1, (width - strlen(FOOTER)) / 2, FOOTER);
   win = newwin(wheight, wwidth, 1, (width - wwidth) / 2);
   refresh();
   /* attroff(A_DIM); */
}

void _drawitems() {
   werase(win);
   box(win, 0, 0);

   cmd_t cmd = init_cmd("transmission-remote -l 2> /dev/null");
   items = cmd.outputs;
   count = cmd.lines;
   end = count > wheight - 2 ? wheight - 2 : count;

   for (i = 1, j = start; j < end; ++i, ++j) {
      if (i - 1 == mark) {
         wattron(win, A_REVERSE);
      } else if (strstr(items[j], "   100%")) {
         wattron(win, COLOR_PAIR(COMPLETED_PAIR));
      } else if (strstr(items[j], "   Stopped")) {
         wattron(win, COLOR_PAIR(STOPPED_PAIR));
      } else {
         wattron(win, COLOR_PAIR(RUNNING_PAIR));
      }

      mvwaddnstr(win, i, 1, items[j], wwidth - 2);

      wattroff(win, A_REVERSE);
      wattroff(win, COLOR_PAIR(COMPLETED_PAIR));
      wattroff(win, COLOR_PAIR(STOPPED_PAIR));
      wattroff(win, COLOR_PAIR(RUNNING_PAIR));
   }
   /* resize_term(height, width); */
}

void handleinput() {
   int key;
   char cmd[1024];
   char* head = "transmission-remote";
   char* tail = "> /dev/null 2>&1";

   halfdelay(10);
   while (1) {
      _drawitems();
      key = wgetch(win);
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
      } else if (key == 'l') {
         sprintf(cmd, "%s -t %.10s -s %s", head, items[mark], tail);
         system(cmd);
      } else if (key == 'h') {
         sprintf(cmd, "%s -t %.10s -S %s", head, items[mark], tail);
         system(cmd);
      } else if (key == 'd') {
         sprintf(cmd, "%s -t %.10s -rad %s", head, items[mark], tail);
         system(cmd);
         mark = mark > 0 ? mark - 1 : 0;
      } else if (key == 'q') {
         break;
      }
   }
}

void cleanup() {
   for (i = 0; i < count; ++i) free(items[i]);
   delwin(win);
   free(items);
   endwin();
}
