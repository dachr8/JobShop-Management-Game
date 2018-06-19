#include "jobshop.h"

void draw_gantt_cb(void *s) {
    if (line)
        Fl::delete_widget(line);

    ((Fl_Scroll *) s)->begin();//begin to edit scroll window
    //according to machine's order ,finish the gantt graph
    for (int i = 0; i < machineNum; ++i) {
        MACHINEPTR node = machine[i];
        for (int j = 0; j < jobNum; ++j) {
            Fl_Box *box = new Fl_Box(line_status * 100 + node->startTime * 3, i * 75,
                                     (node->endTime - node->startTime) * 3, 28);
            std::string str = std::to_string(node->job) + "-" + std::to_string(node->order) + "\n" +
                              std::to_string(node->startTime) + "-" + std::to_string(node->endTime);
            box->copy_label(str.data());
            box->box(FL_UP_BOX);
            box->color((Fl_Color) node->job + 1);
            box->labelcolor(FL_BLACK);
            node = node->nextJob;
        }
    }
    Fl_Box *box = new Fl_Box(0, machineNum * 75, 1, 25);
    box->box(FL_NO_BOX);


    if (overhaul) {
        OVERHAULPTR tmp = NULL;
        for (int i = 0; i < machineNum; i++) {
            if (overhaul[i]) {
                tmp = overhaul[i];
                for (int j = 0; tmp; j++) {
                    Fl_Box *box = new Fl_Box(line_status * 100 + tmp->startTime * 3, i * 75,
                                             (tmp->endTime - tmp->startTime) * 3, 28);
                    std::string str = "Overhaul-" + std::to_string(i) + "\n" + std::to_string(tmp->startTime) + "-" +
                                      std::to_string(tmp->endTime);
                    box->copy_label(str.data());
                    box->box(FL_UP_BOX);
                    box->color((Fl_Color) FL_BLACK);
                    box->labelcolor(FL_BLACK);
                    tmp = tmp->nextOverhaul;
                }
            }
        }
    }
    ((Fl_Scroll *) s)->end();
    ((Fl_Scroll *) s)->show();

    if (line_status == ON) {
        line = new Fl_Box(100, 0, 2, window_gantt->h() - 125);
        line->box(FL_UP_BOX);
        line->color((Fl_Color) FL_BLACK);
    }
};
