/*
 * tide
 * Minimal Transmission CLI client
 */

#include "pthread.h"
#include "ui.h"

int main(void) {
   init_ui();
   draw_ui();
   pthread_t thread_1, thread_2;
   pthread_create(&thread_1, NULL, get_torrents, NULL);
   pthread_create(&thread_2, NULL, handle_input, NULL);
   pthread_exit(NULL);
   return 0;
}
