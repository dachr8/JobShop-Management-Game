#include "jobshop.h"

void start_cb(Fl_Widget *button) {
    if (start_status == OFF) {
        line_status = ON;
        std::thread d(draw_gantt_cb, scroll2);
        d.detach();
        start_status = ON;
    }
    timeline = 0;
    move_status = ON;
    button->label("Restart");
}
