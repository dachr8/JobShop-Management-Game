#include "jobshop.h"

void button_cb(Fl_Widget *button) {
    if (!jobNum)
        get_input_cb();
    else
        window_gantt->show();
}
