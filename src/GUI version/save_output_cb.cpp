#include "jobshop.h"

void save_output_cb() {
    if (jobNum && schedule_thread_status == OFF) {
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Save File As");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
        fnfc.filter("TXT Files*.txt\t*");
        if (fnfc.show())return;
        freopen(fnfc.filename(), "a", stdout);///
        output(makespan);
        fclose(stdout);
    } else {
        fl_message_title("Error");
        fl_message("No output file can be saved now...");
    }
}
