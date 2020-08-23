#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "../config.h"
#include "torrents.h"
#include "ui.h"

int mark, start, end, wheight, count, i, j;
char server_prefix[64] = {0};
char** items;
WINDOW *win, *header, *footer;

void init_ui(void) {
   initscr();
   cbreak();
   noecho();
   curs_set(0);

   start_color();
   use_default_colors();
   init_pair(Pair_Running, COLOR_YELLOW, -1);
   init_pair(Pair_Stopped, COLOR_RED, -1);
   init_pair(Pair_Completed, COLOR_GREEN, -1);

   mark = start = 0;

   if (REMOTE_USE == 1)
      sprintf(server_prefix, "ssh -p %s %s@%s", REMOTE_PORT, REMOTE_USER,
              REMOTE_IP);
}

void draw_ui(void) {
   clear();

   header = newwin(3, COLS, 0, 0);
   box(header, 0, 0);
   mvwaddnstr(header, 1, 1, HEADER, COLS - 2);
   mvwaddnstr(header, 1, COLS - 7, "[tide]", COLS - 2);
   wrefresh(header);

   wheight = LINES - 4;
   win = newwin(wheight, COLS, 2, 0);
   keypad(win, true);

   footer = newwin(3, COLS, LINES - 3, 0);
   box(footer, 0, 0);
}

void _drawitems(void) {
   char cmd_str[1024];
   Torrents torrents;

   sprintf(cmd_str, "%s %s", server_prefix,
           "transmission-remote -l 2> /dev/null");

   torrents = init_cmd(cmd_str);
   count = torrents.count - 2;
   if (count > 0)
      items = ++torrents.list;
   else
      items = torrents.list;

   end = count > wheight - 2 ? wheight - 2 : count;

   werase(win);
   box(win, 0, 0);
   for (i = 1, j = start; j < end; ++i, ++j) {
      if (i - 1 == mark)
         wattron(win, A_REVERSE);
      else if (strstr(items[j], "   100%"))
         wattron(win, COLOR_PAIR(Pair_Completed));
      else if (strstr(items[j], "  Stopped"))
         wattron(win, COLOR_PAIR(Pair_Stopped));
      else
         wattron(win, COLOR_PAIR(Pair_Running));

      mvwaddnstr(win, i, 1, items[j], COLS - 2);

      wattroff(win, A_REVERSE);
      wattroff(win, COLOR_PAIR(Pair_Completed));
      wattroff(win, COLOR_PAIR(Pair_Stopped));
      wattroff(win, COLOR_PAIR(Pair_Running));
   }

   mvwaddnstr(footer, 1, 1, items[count], COLS - 2);
   wrefresh(footer);
}

void _send_args(char* arg) {
   char cmd[1024];
   if (count > 0) {
      sprintf(cmd, "%s transmission-remote -t %.10s %s > /dev/null 2>&1",
              server_prefix, items[mark], arg);
      system(cmd);
   }
}

void handle_input(void) {
   int key;

   halfdelay(10);
   while (1) {
      _drawitems();
      key = wgetch(win);
      if (key == TOR_DOWN) {
         if (mark < wheight - 3 && mark < end - 1) {
            ++mark;
         } else if (end < count) {
            ++end;
            ++start;
         } else {
            end = count > wheight - 2 ? wheight - 2 : count;
            start = mark = 0;
         }
      } else if (key == TOR_UP) {
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
      } else if (key == TOR_START) {
         _send_args("-s");
      } else if (key == TOR_STOP) {
         _send_args("-S");
      } else if (key == TOR_DELETE) {
         _send_args("-rad");
         mark = mark > 0 ? mark - 1 : 0;
      } else if (key == KEY_RESIZE) {
         draw_ui();
      } else if (key == TOR_QUIT) {
         break;
      }
   }
}

void housekeep(void) { endwin(); }
