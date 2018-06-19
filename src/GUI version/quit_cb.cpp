#include "jobshop.h"

void quit_cb(Fl_Widget *w) {
    int hotspot = fl_message_hotspot();
    fl_message_hotspot(0);
    fl_message_title("Exit");
    int rep = fl_ask("Are you sure you want exit?");
    fl_message_hotspot(hotspot);
    if (rep == 1) {
        freeAll();
        exit(0);
    }
}
