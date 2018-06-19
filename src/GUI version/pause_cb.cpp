#include "jobshop.h"

void pause_cb(Fl_Widget *button) {
    if (move_status == ON)
        move_status = OFF;
    else
        move_status = ON;
}
