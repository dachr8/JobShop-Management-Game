#ifndef JOBSHOP_MANAGEMENT_GAME_JOBSHOP_H
#define JOBSHOP_MANAGEMENT_GAME_JOBSHOP_H

//C
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//C++
#include <iostream>
#include <string>
#include <thread>


//fltk-1.3.4-2
#include <FL/Fl.H>
#include <FL/math.h>
#include <FL/fl_ask.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Native_File_Chooser.H>


//Algorithm
#define SIZE (4 * jobNum + 4 * machineNum + 40)
#define DOOM (jobNum + 5)
#define LIMIT 300000


//GUI
#define ON 1
#define OFF 0

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define MENU_HEIGHT 30
#define TITLE_HEIGHT 90

#define DIVIDE 20

#define MAKESPAN_WIDTH 200
#define MAKESPAN_HEIGHT 40

#define BUTTON_WIDTH 140
#define BUTTON_HEIGHT 40
#define BUTTON_LIBELSIZE 18

#define INPUT_HEIGHT 30


//Data Struct
typedef struct job {
    int machine, time;
    struct job *nextMachine;
} *JOBPTR;

typedef struct machine {
    int job, order, startTime, endTime;
    struct machine *nextJob;
} *MACHINEPTR;

typedef struct overhaul {
    int startTime, endTime;
    struct overhaul *nextOverhaul;
} *OVERHAULPTR;


//Global Variables
JOBPTR *job;
MACHINEPTR *machine;
OVERHAULPTR *overhaul;

int jobNum, machineNum, makespan, len, **population, timeline;
int schedule_thread_status = OFF, move_status = OFF, start_status = OFF, line_status = OFF;
double usedTime;

//Global Variables - fltk-1.3.4-2
Fl_Double_Window *window_main, *window_information, *window_gantt, *window_overhaul;
Fl_Multiline_Output *information;
Fl_Scroll *scroll, *scroll2;
Fl_Box *box_makespan, *line;
Fl_Return_Button *button;
Fl_Int_Input *input;
Fl_Spinner *spinner;
Fl_Group *shadiao;


//CLI Function
void *getJob();

int schedule();

void output(int makespan);

void freeAll();


//CLI Function - Genetic Algorithm
void swap(int *a, int *b);

void swapPtr(int **a, int **b);

void initPopulation();

int *crossover(const int *a, const int *b);

int computeDAGAndStartTime(const int *chromosome, int mode);


//GUI Function
void project_information_cb();

void input_information_cb();

void save_output_cb();

void quit_cb(Fl_Widget *w);

void hide_cb(Fl_Widget *o);

void draw_gantt_cb(void *s);

void start_cb(Fl_Widget *button);

void pause_cb(Fl_Widget *button);

void schedule_thread();

void get_input_cb();

void button_cb(Fl_Widget *button);

void overhaul_cb(Fl_Widget *button);

void overhaul_confirm_cb(Fl_Widget *button);

void overhaul_thread();

void move_cb(void *box_time);


//Drawing Sand Sculpture
class Drawing : public Fl_Widget {
    void draw();

public:
    Drawing(int X, int Y, int W, int H, const char *L) : Fl_Widget(X, Y, W, H, L) {
        align(FL_ALIGN_TOP);
        box(FL_FLAT_BOX);
        color(FL_WHITE);
    }
};

void Drawing::draw() {
    draw_box();
    fl_push_matrix();
    fl_translate(x() + w() / 2, y() + h() / 2);
    fl_scale(w() / 2, h() / 2);
    fl_color(FL_BLACK);
    for (int i = 0; i < 20; i++) {
        for (int j = i + 1; j < 20; j++) {
            fl_begin_line();
            fl_vertex(cos(M_PI * i / 10 + .1), sin(M_PI * i / 10 + .1));
            fl_vertex(cos(M_PI * j / 10 + .1), sin(M_PI * j / 10 + .1));
            fl_end_line();
        }
    }
    fl_pop_matrix();
}

#endif //JOBSHOP_MANAGEMENT_GAME_JOBSHOP_H
