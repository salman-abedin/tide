#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"
#include "ui.h"

int mark, start, end, wwidth, wheight, count, i, j;
char** items;
WINDOW *win, *header;

void init_ui(void)
{
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
}

void draw_ui(void)
{
   wwidth = COLS;
   wheight = LINES - 4;

   clear();

   int hwidth = strlen(HEADER) + 8;
   header = newwin(3, hwidth, 0, (COLS - hwidth) / 2);
   box(header, 0, 0);
   mvwprintw(header, 1, 4, HEADER);

   mvprintw(LINES - 1, (COLS - strlen(FOOTER)) / 2, FOOTER);

   win = newwin(wheight, wwidth, 3, (COLS - wwidth) / 2);

   refresh();
   wrefresh(header);
}

void _drawitems(void)
{
   cmd_t cmd = init_cmd("transmission-remote -l 2> /dev/null");

   items = cmd.outputs;
   count = cmd.lines;
   end = count > wheight - 2 ? wheight - 2 : count;

   werase(win);
   box(win, 0, 0);
   for (i = 1, j = start; j < end; ++i, ++j) {
      if (i - 1 == mark)
         wattron(win, A_REVERSE);
      else if (strstr(items[j], "   100%"))
         wattron(win, COLOR_PAIR(COMPLETED_PAIR));
      else if (strstr(items[j], "Stopped"))
         wattron(win, COLOR_PAIR(STOPPED_PAIR));
      else
         wattron(win, COLOR_PAIR(RUNNING_PAIR));

      mvwaddnstr(win, i, 1, items[j], wwidth - 2);

      wattroff(win, A_REVERSE);
      wattroff(win, COLOR_PAIR(COMPLETED_PAIR));
      wattroff(win, COLOR_PAIR(STOPPED_PAIR));
      wattroff(win, COLOR_PAIR(RUNNING_PAIR));
   }
}

void _send_args(char* arg)
{
   char cmd[256];
   sprintf(cmd,
       "%s -t %.10s %s %s",
       "transmission-remote",
       items[mark],
       arg,
       "> /dev/null 2>&1");
   system(cmd);
}

void handle_input(void)
{
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

void housekeep(void)
{
   for (i = 0; i < count; ++i)
      free(items[i]);
   free(items);
   delwin(win);
   delwin(header);
   endwin();
}
