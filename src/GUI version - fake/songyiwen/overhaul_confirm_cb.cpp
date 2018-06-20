#include "jobshop.h"

void overhaul_confirm_cb(Fl_Widget *button) {
    if (atoi(input->value()) > 0) {
        std::thread o(overhaul_thread);
        o.detach();
        window_overhaul->hide();
    } else
        fl_alert("Time is invalid");
}
