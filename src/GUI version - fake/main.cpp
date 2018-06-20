#include "jobshop.h"

int main(int argc, char **argv) {
    Fl::scheme("plastic");

    window_main = new Fl_Double_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "JobShop Management Game");

    Fl_Menu_Bar menu(0, 0, window_main->w(), MENU_HEIGHT);
    Fl_Menu_Item menuitems[] = {
            {"&File",              0, 0,                                    0, FL_SUBMENU},
            {"&Import Input File", 0, (Fl_Callback *) get_input_cb,         0, FL_MENU_DIVIDER},
            {"&Check Input File",  0, (Fl_Callback *) input_information_cb, 0, FL_MENU_DIVIDER},
            {"&Save File As...",
                                   FL_COMMAND + FL_SHIFT + 's', (Fl_Callback *) save_output_cb, 0, FL_MENU_DIVIDER},//
            {"&Exit", FL_COMMAND + 'q',            (Fl_Callback *) quit_cb},
            {0},
            {"&About",             0, 0,                                    0, FL_SUBMENU},
            {"About The &Project", 0, (Fl_Callback *) project_information_cb},
            {0},
            {0}
    };
    menu.copy(menuitems);

    int title_Y = MENU_HEIGHT + DIVIDE;
    int title_width = window_main->w() - DIVIDE * 2;
    Fl_Box title(DIVIDE, title_Y, title_width, TITLE_HEIGHT, "JobShop Management Game");
    title.box(FL_UP_BOX);
    title.labelsize(36);
    title.labelfont(FL_BOLD + FL_ITALIC);
    title.labeltype(FL_SHADOW_LABEL);


    Fl_Group gantt(0, 175, 800, 300);//create the scroll window
    scroll = new Fl_Scroll(0, 175, 800, 300);
    scroll->end();
    scroll->hide();//avoid showing the scroll now
    gantt.end();


    int makespan_X = (window_main->w() - MAKESPAN_WIDTH) / 2;
    int makespan_Y = window_main->h() - MAKESPAN_HEIGHT - BUTTON_HEIGHT - DIVIDE * 2;
    box_makespan = new Fl_Box(makespan_X, makespan_Y, MAKESPAN_WIDTH, MAKESPAN_HEIGHT,
                              "Please click the button ↓↓↓");
    box_makespan->labelsize(20);
    box_makespan->labelfont(FL_BOLD);
    box_makespan->labeltype(FL_SHADOW_LABEL);

    int button_X = (window_main->w() - BUTTON_WIDTH) / 2;
    int button_Y = window_main->h() - BUTTON_HEIGHT - DIVIDE;
    button = new Fl_Return_Button(button_X, button_Y, BUTTON_WIDTH, BUTTON_HEIGHT, "Start");
    button->labelsize(BUTTON_LIBELSIZE);
    button->labelfont(FL_BOLD);
    button->labeltype(FL_SHADOW_LABEL);
    button->callback(button_cb);

    shadiao = new Fl_Group(window_main->w() / 2 - 160, 160, 320, 320, 0);
    Drawing drawing(window_main->w() / 2 - 160, 160, 320, 320, 0);
    shadiao->end();

    window_main->resizable(gantt);//window->resizable(scroll);
    window_main->callback(quit_cb);
    window_main->end();//We tell FLTK that we will not add any more widgets to window.
    window_main->show(argc, argv);//Finally, we show the window and enter the FLTK event loop:




    window_information = new Fl_Double_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "Input Information");
    information = new Fl_Multiline_Output(DIVIDE, DIVIDE, window_information->w() - 2 * DIVIDE,
                                          window_information->h() - 2 * DIVIDE);
    window_information->resizable(information);
    window_information->callback(hide_cb);
    window_information->end();//We tell FLTK that we will not add any more widgets to window.
    window_information->hide();


    window_gantt = new Fl_Double_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "Gantt chart");

    Fl_Group gantt2(0, 0, 800, 475);
    scroll2 = new Fl_Scroll(0, 0, 800, 475);
    scroll2->type(Fl_Scroll::VERTICAL_ALWAYS);
    //draw_gantt_cb(scroll2);
    scroll2->end();
    scroll2->hide();
    gantt2.end();


    std::string tmp = "Timeline: 0\nMakespan: " + std::to_string(makespan);
    Fl_Box *box_time = new Fl_Box(makespan_X, makespan_Y, MAKESPAN_WIDTH, MAKESPAN_HEIGHT, tmp.data());
    box_time->labelsize(20);
    box_time->labelfont(FL_BOLD);
    box_time->labeltype(FL_SHADOW_LABEL);

    int button_left_X = window_gantt->w() / 2 - BUTTON_WIDTH * 2;
    int button_middle_X = (window_gantt->w() - BUTTON_WIDTH) / 2;
    int button_right_X = window_gantt->w() / 2 + BUTTON_WIDTH;
    Fl_Button button_start(button_left_X, button_Y, BUTTON_WIDTH, BUTTON_HEIGHT, "Start");
    button_start.labelsize(BUTTON_LIBELSIZE);
    button_start.labelfont(FL_BOLD);
    button_start.labeltype(FL_SHADOW_LABEL);
    button_start.callback(start_cb);

    Fl_Button button_pause(button_middle_X, button_Y, BUTTON_WIDTH, BUTTON_HEIGHT, "Pause");
    button_pause.labelsize(BUTTON_LIBELSIZE);
    button_pause.labelfont(FL_BOLD);
    button_pause.labeltype(FL_SHADOW_LABEL);
    button_pause.callback(pause_cb);

    Fl_Return_Button button_overhaul(button_right_X, button_Y, BUTTON_WIDTH, BUTTON_HEIGHT, "Overhaul");
    button_overhaul.labelsize(BUTTON_LIBELSIZE);
    button_overhaul.labelfont(FL_BOLD);
    button_overhaul.labeltype(FL_SHADOW_LABEL);
    button_overhaul.callback(overhaul_cb);


    window_gantt->resizable(gantt);
    window_gantt->callback(quit_cb);
    window_gantt->end();//We tell FLTK that we will not add any more widgets to window.
    window_gantt->hide();


    window_overhaul = new Fl_Double_Window(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, "Overhaul");

    spinner = new Fl_Spinner(WINDOW_WIDTH / 10, 30, 200, INPUT_HEIGHT, "Machine:");
    spinner->minimum(0);
    spinner->maximum(machineNum - 1);
    spinner->step(1);
    spinner->value(0);

    input = new Fl_Int_Input(WINDOW_WIDTH / 10, 130, 200, INPUT_HEIGHT, "Time:");

    Fl_Button button_overhaul_confirm(WINDOW_WIDTH / 10, 230, BUTTON_WIDTH, BUTTON_HEIGHT, "&Confirm");
    button_overhaul_confirm.labelsize(BUTTON_LIBELSIZE);
    button_overhaul_confirm.labelfont(FL_BOLD);
    button_overhaul_confirm.labeltype(FL_SHADOW_LABEL);
    button_overhaul_confirm.callback(overhaul_confirm_cb);


    window_overhaul->callback(hide_cb);
    window_overhaul->end();
    window_overhaul->hide();


    timeline = 0;
    Fl::add_timeout(0.1, move_cb, box_time);


    return Fl::run();
}
