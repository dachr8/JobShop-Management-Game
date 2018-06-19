#include "jobshop.h"

void get_input_cb() {
    if (schedule_thread_status == OFF && line_status == OFF) {
        freeAll();
        scroll->hide();
        overhaul = NULL;

        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open file");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        fnfc.filter("TXT Files*.txt\t*");
        if (fnfc.show())return;
        freopen(fnfc.filename(), "r", stdin);///
        getJob();
        fclose(stdin);

        shadiao->show();
        box_makespan->label("Calculating...");
        button->hide();

        schedule_thread_status = ON;
        std::thread s(schedule_thread);
        s.detach();
    } else {
        fl_message_title("Error");
        fl_message("No input file can be loaded now...");
    }
}
