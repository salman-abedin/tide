#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "../config.h"
#include "cmd.h"
#include "ui.h"

int mark, start, end, wheight, count, i, j;
char** items;
WINDOW *win, *banner, *summary, *header;
char server_prefix[256] = {0};

void init_ui(void) {
   initscr();
   cbreak();
   noecho();
   curs_set(0);

   start_color();
   use_default_colors();
   init_pair(RUNNING_PAIR, COLOR_YELLOW, -1);
   init_pair(STOPPED_PAIR, COLOR_RED, -1);
   init_pair(COMPLETED_PAIR, COLOR_GREEN, -1);

   mark = start = 0;

   if (REMOTE_USE == 1)
      sprintf(server_prefix, "ssh -p %s %s@%s", REMOTE_PORT, REMOTE_USER,
              REMOTE_IP);
}

void draw_ui(void) {
   clear();

   int banner_width = strlen(BANNER) + 8;
   banner = newwin(3, banner_width, 0, (COLS - banner_width) / 2);
   box(banner, 0, 0);
   mvwprintw(banner, 1, 4, BANNER);

   header = newwin(3, COLS, 3, 0);
   /* box(header, 0, 0); */
   wborder(header, 0, 0, 0, 1, 0, 0, 1, 1);
   mvwprintw(header, 1, 1, HEADER);

   mvprintw(LINES - 1, (COLS - strlen(BINDINGS)) / 2, BINDINGS);

   wheight = LINES - 7;
   win = newwin(wheight, COLS, 6, 0);

   refresh();
   wrefresh(banner);
   wrefresh(header);
}

void _drawitems(void) {
   char cmd_str[1024];
   sprintf(cmd_str, "%s %s", server_prefix,
           "transmission-remote -l 2> /dev/null");
   cmd_t cmd = init_cmd(cmd_str);

   items = ++cmd.outputs;
   count = cmd.lines - 2;
   end = count > wheight - 2 ? wheight - 2 : count;

   werase(win);
   box(win, 0, 0);
   for (i = 1, j = start; j < end; ++i, ++j) {
      if (i - 1 == mark)
         wattron(win, A_REVERSE);
      else if (strstr(items[j], "   100%"))
         wattron(win, COLOR_PAIR(COMPLETED_PAIR));
      else if (strstr(items[j], "  Stopped"))
         wattron(win, COLOR_PAIR(STOPPED_PAIR));
      else
         wattron(win, COLOR_PAIR(RUNNING_PAIR));

      mvwaddnstr(win, i, 1, items[j], COLS - 2);

      wattroff(win, A_REVERSE);
      wattroff(win, COLOR_PAIR(COMPLETED_PAIR));
      wattroff(win, COLOR_PAIR(STOPPED_PAIR));
      wattroff(win, COLOR_PAIR(RUNNING_PAIR));
   }
}

void _send_args(char* arg) {
   char cmd[1024];
   sprintf(cmd, "%s transmission-remote -t %.10s %s > /dev/null 2>&1",
           server_prefix, items[mark], arg);
   system(cmd);
}

void handle_input(void) {
   int key;

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
         _send_args("-s");
      } else if (key == 'h') {
         _send_args("-S");
      } else if (key == 'd') {
         _send_args("-rad");
         mark = mark > 0 ? mark - 1 : 0;
      } else if (key == KEY_RESIZE) {
         draw_ui();
      } else if (key == 'q') {
         break;
      }
   }
}

void housekeep(void) {
   delwin(win);
   delwin(banner);
   endwin();
}
