#include "jobshop.h"

void schedule_thread() {
    double startTime = clock();

    makespan = schedule();

    usedTime = (clock() - startTime) / CLOCKS_PER_SEC;

    // The following part is aimed to
    // convert a float in to a string with 3 significant digits
    char t[15];
    sprintf(t, "%.3lf", usedTime);
    std::string tmp(t), str = "Used time: " + tmp + " (s)\nMakespan: " + std::to_string(makespan);

    shadiao->hide();
    draw_gantt_cb(scroll);//after finish schedule ,show the scroll and gantt graph

    static const char *button_label = str.data();
    box_makespan->label(button_label);

    button->label("Next");
    button->show();

    schedule_thread_status = OFF;

    save_output_cb();
}
