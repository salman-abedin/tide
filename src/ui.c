#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../config.h"
#include "_torrents.h"
#include "ui.h"

int mark, start, end, count, old_count, i, j;
char** items;
char cmd_str[1024];
WINDOW* win;
Torrents torrents;

void _draw_torrents() {
   torrents = init_cmd(cmd_str);
   count = torrents.count - 2;

   if (old_count != count) {
      old_count = count;
      end = count > LINES - 6 ? LINES - 6 : count;
   }

   if (count > 0)
      items = ++torrents.list;
   else
      items = torrents.list;
}

void init_ui(void) {
   initscr();
   cbreak();
   noecho();
   curs_set(0);

   sprintf(cmd_str, "transmission-remote -l 2> /dev/null");

   start_color();
   use_default_colors();
   init_pair(Pair_Running, COLOR_YELLOW, -1);
   init_pair(Pair_Stopped, COLOR_RED, -1);
   init_pair(Pair_Completed, COLOR_GREEN, -1);

   mark = start = 0;

   old_count = init_cmd(cmd_str).count - 2;
   end = old_count > LINES - 6 ? LINES - 6 : old_count;

   _draw_torrents();
}

void draw_ui(void) {
   clear();

   win = newwin(LINES, COLS, 0, 0);
   keypad(win, true);
}

void _draw_items(void) {
   werase(win);
   box(win, 0, 0);

   mvwaddnstr(win, 1, 1, HEADER, COLS - 2);
   mvwaddnstr(win, 1, COLS - 7, "[tide]", COLS - 2);
   wmove(win, 2, 1);
   whline(win, 0, COLS - 2);

   for (i = 3, j = start; j < end; ++i, ++j) {
      if (i - 3 == mark)
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

   wmove(win, LINES - 3, 1);
   whline(win, 0, COLS - 2);
   mvwaddnstr(win, LINES - 2, 1, count == 0 ? "" : items[count], COLS - 2);

   refresh();
   wrefresh(win);
}

void _send_args(char* arg) {
   int id;
   char cmd[1024];

   sscanf(items[mark], "%d", &id);
   if (count > 0) {
      sprintf(cmd, "transmission-remote -t %d %s > /dev/null 2>&1", id, arg);
      system(cmd);
   }
}

void* handle_input() {
   int key;
   while ((key = wgetch(win)) != TOR_QUIT) {
      if (key == TOR_DOWN) {
         if (mark < LINES - 7 && mark < end - 1) {
            ++mark;
         } else if (end < count) {
            ++end;
            ++start;
         } else {
            end = count > LINES - 6 ? LINES - 6 : count;
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
               mark = count > LINES - 2 ? LINES - 3 : count - 1;
               start = count > LINES - 2 ? count - LINES + 2 : 0;
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
      }
      _draw_items();
   }
   housekeep();
   exit(0);
}

void* get_torrents() {
   while (1) {
      _draw_torrents();
      _draw_items();
      sleep(1);
   }
}

void housekeep(void) { endwin(); }
