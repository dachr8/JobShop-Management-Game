#include "jobshop.h"

void move_cb(void *box_time) {
    std::string tmp = "Timeline: " + std::to_string(timeline) + "\nMakespan: " + std::to_string(makespan);
    ((Fl_Box *) box_time)->label(tmp.data());
    if (move_status == ON) {
        scroll2->hide();
        scroll2->scroll_to(timeline * 3 - 100, scroll2->yposition());
        timeline += 1;
        scroll2->show();
    }
    Fl::repeat_timeout(0.1, move_cb, box_time);
}
