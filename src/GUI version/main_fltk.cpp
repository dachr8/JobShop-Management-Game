#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Pack.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Multiline_Output.H>

#include <iostream>
#include <string>
#include <thread>

#include "jobshop.h"
#include "itos.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define MENU_HEIGHT 30
#define TITLE_HEIGHT 90

#define DIVIDE 20

//#define PACK_DIVIDE 200

//#define INPUT_HEIGHT 30

#define MAKESPAN_WIDTH 200
#define MAKESPAN_HEIGHT 40

#define BUTTON_WIDTH 120
#define BUTTON_HEIGHT 40

void *getJob();

int schedule();

void output(int makespan);

void freeAll();

Fl_Double_Window *window, *window_information;
Fl_Multiline_Output *information;
Fl_Box *box_makespan;
Fl_Return_Button *button;

void schedule_thread() {
    double startTime = clock();

    thread_status = 1;
    makespan = schedule();
    thread_status = 0;

    usedTime = (clock() - startTime) / CLOCKS_PER_SEC;
    std::string tmp = "Used time: " + std::to_string(usedTime) + "\nMakespan: " + std::to_string(makespan);
    box_makespan->label(tmp.data());
    button->label("Next");
    button->show();
}

void get_input_cb() {
    if (!thread_status) {
        freeAll();
        overhaul = NULL;

        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open file");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        if (fnfc.show())return;
        freopen(fnfc.filename(), "r", stdin);///
        getJob();
        fclose(stdin);

        box_makespan->label("Calculating...");
        button->hide();

        std::thread t(schedule_thread);
        t.detach();
    } else {
        fl_message_title("Error");
        fl_message("No input file can be loaded now...");
    }

}

void input_information_cb() {
//    std::ostringstream ostr;
//    ostr << std::to_string(jobNum) << " " << std::to_string(machineNum) << std::endl;
//    std::string tmp = ostr.str();
    if (jobNum) {
        std::string tmp = std::to_string(jobNum) + "\t" + std::to_string(machineNum) + "\n";
        for (int i = 0; i < jobNum; ++i) {
            JOBPTR node = job[i];
            for (int j = 0; j < machineNum - 1; ++j) {
                tmp += std::to_string(node->machine) + " " + std::to_string(node->time) + "\t";
                node = node->nextMachine;
            }
            tmp += std::to_string(node->machine) + " " + std::to_string(node->time) + "\n";
        }
        information->value(tmp.data());
    }
    window_information->show();
}

void save_output_cb(const char *filename) {
    if (jobNum && !thread_status) {
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Save File As");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        if (fnfc.show())return;
        freopen(fnfc.filename(), "a", stdout);///
        output(makespan);
        fclose(stdout);
    } else {
        fl_message_title("Error");
        fl_message("No output file can be saved now...");
    }
}

void quit_cb(Fl_Widget *o) {
    int hotspot = fl_message_hotspot();
    fl_message_hotspot(0);
    fl_message_title("Exit");
    int rep = fl_ask("Are you sure you want exit?");
    fl_message_hotspot(hotspot);
    if (rep == 1) {
        freeAll();
        exit(0);
    }
}

void hide_cb(Fl_Widget *o) {
    o->hide();
}


void project_information_cb() {
    fl_message_title("Introduction");
    fl_message("Group: 9~12班36\n\n"
               "Authors: 张志博 赵佳祺 宋伊雯\n\n"
               "Algorithm: Genetic algorithm\n\n"
               "Graphical User Interface: Fast Light Tool Kit (\"FLTK\") version 1.3.4\n");
}


void button_cb(Fl_Widget *o) {
    if (!jobNum)
        get_input_cb();
    else;//@aojia
}

int main(int argc, char **argv) {
    window = new Fl_Double_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "JobShop Management Game");
    window->size_range(window->w(), window->h(), 0, 0);

    Fl::scheme("plastic");

    Fl_Menu_Bar *menu = new Fl_Menu_Bar(0, 0, window->w(), MENU_HEIGHT);
    Fl_Menu_Item menuitems[] = {
            {"&File",              0, 0,                                    0, FL_SUBMENU},
            {"&Import Input File", 0, (Fl_Callback *) get_input_cb,         0, FL_MENU_DIVIDER},
            {"&Check Input File",  0, (Fl_Callback *) input_information_cb, 0, FL_MENU_DIVIDER},
            {"&Save File As...", FL_COMMAND + FL_SHIFT + 's', (Fl_Callback *) save_output_cb, 0, FL_MENU_DIVIDER},//
            {"&Exit",            FL_COMMAND + 'q',            (Fl_Callback *) quit_cb},
            {0},
            {"&About",             0, 0,                                    0, FL_SUBMENU},
            {"About The &Project", 0, (Fl_Callback *) project_information_cb},
            {0},
            {0}
    };
    menu->copy(menuitems);

    int title_Y = MENU_HEIGHT + DIVIDE;
    int title_width = window->w() - DIVIDE * 2;
    Fl_Box *title = new Fl_Box(DIVIDE, title_Y, title_width, TITLE_HEIGHT, "JobShop Management Game");
    title->box(FL_UP_BOX);
    title->labelsize(36);
    title->labelfont(FL_BOLD + FL_ITALIC);
    title->labeltype(FL_SHADOW_LABEL);

/*
 * 这里显示第一次结果甘特图啦
    int pack_Y = MENU_HEIGHT + DIVIDE * 2 + TITLE_HEIGHT;
    int pack_width = window->w() - PACK_DIVIDE * 2;
    int pack_height = window->h() - pack_Y - DIVIDE * 2 - BUTTON_HEIGHT;
    pack = new Fl_Pack(PACK_DIVIDE, pack_Y, pack_width, pack_height);
    pack->type(Fl_Pack::VERTICAL);//HORIZONTAL
    pack->spacing(DIVIDE);

    input[0] = new Fl_Int_Input(0, 0, 0, INPUT_HEIGHT, "Job number: ");
    input[1] = new Fl_Int_Input(0, 0, 0, INPUT_HEIGHT, "Machine number: ");
    for (int i = 0; i < 2; ++i) {
        input[i]->labelsize(20);
        input[i]->labelfont(FL_BOLD);
        input[i]->labeltype(FL_SHADOW_LABEL);
    }

    pack->end();


*/
    int makespan_X = (window->w() - MAKESPAN_WIDTH) / 2;
    int makespan_Y = window->h() - MAKESPAN_HEIGHT - BUTTON_HEIGHT - DIVIDE * 2;
    box_makespan = new Fl_Box(makespan_X, makespan_Y, MAKESPAN_WIDTH, MAKESPAN_HEIGHT, "Please click the button ↓↓↓");
    box_makespan->labelsize(20);
    box_makespan->labelfont(FL_BOLD);
    box_makespan->labeltype(FL_SHADOW_LABEL);

    int button_X = (window->w() - BUTTON_WIDTH) / 2;
    int button_Y = window->h() - BUTTON_HEIGHT - DIVIDE;
    button = new Fl_Return_Button(button_X, button_Y, BUTTON_WIDTH, BUTTON_HEIGHT, "Start");
    button->labelsize(18);
    button->labelfont(FL_BOLD);
    button->labeltype(FL_SHADOW_LABEL);
    button->callback(button_cb);


    window->resizable(box_makespan);//
    window->callback(quit_cb);
    window->end();//We tell FLTK that we will not add any more widgets to window.
    window->show(argc, argv);//Finally, we show the window and enter the FLTK event loop:



    window_information = new Fl_Double_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "Input Information");
    information = new Fl_Multiline_Output(DIVIDE, DIVIDE, window_information->w() - 2 * DIVIDE,
                                          window_information->h() - 2 * DIVIDE);

    window_information->resizable(information);
    window_information->callback(hide_cb);
    window_information->end();//We tell FLTK that we will not add any more widgets to window.
    window_information->hide();


    return Fl::run();
}



//io.c
//author @dachr
//co-author @aojia
//#include <malloc.h>

//#include "jobshop.h"

void *getJob() {
    scanf("%d%d", &jobNum, &machineNum);

    job = (JOBPTR *) malloc(jobNum * sizeof(JOBPTR));

    for (int i = 0; i < jobNum; ++i) {
        JOBPTR node = job[i] = (JOBPTR) malloc(sizeof(struct job));
        for (int j = 0; j < machineNum - 1; ++j) {
            scanf("%d %d", &node->machine, &node->time);
            node = node->nextMachine = (JOBPTR) malloc(sizeof(struct job));
        }
        scanf("%d %d", &node->machine, &node->time);
        node->nextMachine = NULL;
    }
}

/*
void getOverhaul() {
    overhaul = malloc(machineNum * sizeof(OVERHAULPTR));
    for (int i = 0; i < machineNum; ++i)
        overhaul[i] = NULL;
    int num = 0, startTime, machine, time;
    while (num != -1) {
        while (scanf("%d%d%d", &startTime, &machine, &time) == 3) {
            OVERHAULPTR tmp;
            if (overhaul[machine]) {
                tmp = overhaul[machine];
                while (tmp->nextOverhaul)
                    tmp = tmp->nextOverhaul;
                tmp = tmp->nextOverhaul = malloc(sizeof(struct overhaul));
            }
            else
                tmp = overhaul[machine] = malloc(sizeof(struct overhaul));
            tmp->startTime = startTime;
            tmp->endTime = startTime + time;
            tmp->nextOverhaul = NULL;
        }
        if (scanf("%d", &num) != 1)
            return;
    }
}
*/

void output(int makespan) {
    for (int i = 0, last = 0; i < machineNum; ++i) {
        printf("\nM%d", i);
        MACHINEPTR tmp = machine[i];

        last = 0;
        OVERHAULPTR tmp2 = NULL;


        while (tmp) {
            if (overhaul) {
                tmp2 = overhaul[i];
                int flag = 1;
                while (tmp2 && flag)
                    if (tmp2->endTime <= tmp->startTime && tmp2->startTime >= last) {
                        printf(" (%d,\"overhaul\",%d)", tmp2->startTime, tmp2->endTime);
                        tmp2 = tmp2->nextOverhaul;
                    } else
                        flag = 0;
            }
            printf(" (%d,%d-%d,%d)", tmp->startTime, tmp->job, tmp->order, tmp->endTime);
            last = tmp->startTime;
            tmp = tmp->nextJob;
        }
    }
    printf("\nTime Used: %.3lfs", usedTime);
    printf("\nEnd Time: %d\n\n", makespan);
}

void freeAll() {
    for (int i = 0; i < jobNum; ++i) {
        while (job[i]) {
            JOBPTR tmp = job[i];
            job[i] = job[i]->nextMachine;
            free(tmp);
        }
    }
    free(job);
    for (int i = 0; i < machineNum; ++i) {
        while (machine[i]) {
            MACHINEPTR tmp1 = machine[i];
            machine[i] = machine[i]->nextJob;
            free(tmp1);
        }

        if (overhaul)
            while (overhaul[i]) {
                OVERHAULPTR tmp2 = overhaul[i];
                overhaul[i] = overhaul[i]->nextOverhaul;
                free(tmp2);
            }
    }
    free(machine);
}
//author songyiwen and aojia
//co-author dachr



//schedule.c
//#include "jobshop.h"

void swap(int *a, int *b);

void swapPtr(int **a, int **b);

void initPopulation();

int *crossover(const int *a, const int *b);

int computeDAGAndStartTime(const int *chromosome, int mode);

int schedule() {
    int makespan[SIZE], lastMakespan = 0, doomClock = 0, maxL = 2, survivor = machineNum;
    initPopulation();
    clock_t startClock = clock();
    if (jobNum * machineNum >= 100) {
        ++maxL;
        survivor = 1;
    }
    for (int l = 0; l < maxL && (clock() - startClock < LIMIT / (l + 1)); ++l, doomClock = DOOM) {
        for (int i = 0; i < SIZE * 8; ++i) {//Iterative
            if (doomClock == DOOM) {
                for (int j = survivor; j < SIZE; ++j)
                    swap(&population[j][rand() % len], &population[j][rand() % len]);
                doomClock = 0;
            } else {
                for (int j = 0; j < SIZE / 2; ++j)//Shuffle
                    swapPtr(&population[j], &population[rand() % (SIZE / 2)]);

                for (int j = 0; j < SIZE / 4; ++j) {
                    srand((unsigned) time(NULL));
                    if (rand() % 10 < 7) {
                        free(population[SIZE / 2 + j]);
                        population[SIZE / 2 + j] = crossover(population[j], population[j + SIZE / 4]);//Crossover
                        free(population[3 * SIZE / 4 + j]);
                        population[3 * SIZE / 4 + j] = crossover(population[j + SIZE / 4], population[j]);
                        if (rand() % 10 < 1)//Mutation
                            swap(&population[SIZE / 2 + j][rand() % len], &population[SIZE / 2 + j][rand() % len]);
                        if (rand() % 10 < 1)
                            swap(&population[3 * SIZE / 4 + j][rand() % len],
                                 &population[3 * SIZE / 4 + j][rand() % len]);
                    }
                }
            }

            for (int j = 0; j < SIZE; ++j)
                makespan[j] = computeDAGAndStartTime(population[j], 0);//Get makespan

            for (int j = 0, flag = 1; (j < SIZE - 1) && flag; ++j) {
                flag = 0;
                for (int k = 0; k < SIZE - 1; ++k)//Bubble sort by makespan
                    if (makespan[k] >= makespan[k + 1]) {
                        swap(&makespan[k], &makespan[k + 1]);
                        swapPtr(&population[k], &population[k + 1]);
                        flag = 1;
                    }
            }

            doomClock = makespan[0] == lastMakespan ? doomClock + 1 : 0;
            lastMakespan = makespan[0];
        }
    }

    computeDAGAndStartTime(population[0], 1);//Mode 1 for output

    for (int i = 0; i < SIZE; ++i)
        free(population[i]);
    free(population);

    return makespan[0];
}

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void swapPtr(int **a, int **b) {
    int *t = *a;
    *a = *b;
    *b = t;
}

void initPopulation() {
    len = jobNum * machineNum;
    //Code the workpiece process to initialize the chromosome
    int *p = (int *) malloc(sizeof(int) * len);
    for (int i = 0, k = 0; i < jobNum; ++i)
        for (int j = 0; j < machineNum; ++j)
            p[k++] = i;
    //Perform SIZE operations on chromosomes to establish initial populations
    population = (int **) malloc(sizeof(p) * SIZE);
    for (int i = 0; i < SIZE; ++i)
        population[i] = (int *) malloc(sizeof(int) * len);

    srand((unsigned) time(NULL));
    for (int i = 0; i < SIZE; i++) {//Randomly disrupted gene sequence, len is the length of the p chromosome
        for (int j = 0; j < len; ++j) {
            swap(&p[j], &p[rand() % len]);
            for (int k = 0; k < len; ++k)
                population[i][k] = p[k];
        }
    }
    free(p);
}

int *crossover(const int *a, const int *b) {
    int *child = (int *) malloc(sizeof(int) * len), indexA[len], indexB[len];
    for (int i = 0; i < len; ++i)
        indexA[i] = indexB[i] = 0;
    for (int i = 0; i < len; ++i)
        for (int j = 0; j < i; ++j) {
            if (a[j] == a[i])
                ++indexA[i];
            if (b[j] == b[i])
                ++indexB[i];
        }

    int startA = rand() % len, endA = startA + rand() % (len - startA), posB = rand() % (len + startA - endA);
    for (int i = 0, j = 0; i < len; ++i)
        if (posB <= i && i <= posB + endA - startA)
            child[i] = a[startA + i - posB];
        else
            for (int flag = 1; flag; ++j) {
                flag = 1;
                for (int k = startA; k <= endA && flag == 1; ++k)
                    if (a[k] == b[j] && indexA[k] == indexB[j])
                        flag = 2;
                if (flag == 1) {
                    child[i] = b[j];
                    flag = 0;
                }
            }

    return child;
}

struct graph {
    int point;
    int machine;
    int tmpTime;
    struct graph *ptrA;
    struct graph *ptrB[15];
};

int computeDAGAndStartTime(const int *chromosome, int mode) {
    int T[jobNum];//构建长度为工件数的全0数组，操作累加器
    int tasksResource[machineNum][jobNum];//存放上一次使用这台机器的节点相对工序号
    int startTime[jobNum][machineNum];//各节点的起始时间，初始化为0
    for (int i = 0; i < machineNum; ++i)
        for (int j = 0; j < jobNum; ++j)
            tasksResource[i][j] = -1;
    struct graph G[jobNum][machineNum];

    for (int i = 0; i < jobNum; ++i) {
        T[i] = 0;
        for (int j = 0; j < machineNum; ++j) {
            startTime[i][j] = 0;
            G[i][j].point = 0;
            G[i][j].tmpTime = 0;
            G[i][j].machine = 0;
            G[i][j].ptrA = NULL;
            for (int k = 0; k < 21; ++k)
                G[i][j].ptrB[k] = NULL;
        }
    }

    for (int i = 0, t = 0, max, num, flag[jobNum]; i < len; ++i, ++T[num]) {//对染色体进行遍历及处理
        num = chromosome[i];//num为工件号
        JOBPTR tmpPtr = job[num];//寻找工件及工序所用的临时指针
        t = T[num];

        for (int j = 0; j < t; j++)
            tmpPtr = tmpPtr->nextMachine;
        int r = tmpPtr->machine;//r为对应工件、对应工序的机器号

        G[num][t].tmpTime = tmpPtr->time;
        G[num][t].point = i;
        G[num][t].machine = r;

        if (t < machineNum - 1)
            G[num][t].ptrA = &G[num][t + 1];//若不是num工件的最后一道工序，则将当前工序指向后一道工序的节点i
        else
            G[num][t].ptrA = NULL;//指向最后节点
        //以上为工件约束连接和取值操作

        max = t == 0 ? 0 : startTime[num][t - 1];
        for (int p = 0; p < jobNum; p++)
            flag[p] = 1;
        for (int p = 0, o = 0; p < i; ++p)//p为染色体的操作数
            if (tasksResource[r][chromosome[p]] > -1 && flag[chromosome[p]]) {
                for (o = 0; G[chromosome[p]][tasksResource[r][chromosome[p]]].ptrB[o]; ++o);
                G[chromosome[p]][tasksResource[r][chromosome[p]]].ptrB[o] = &G[num][t];
                flag[chromosome[p]] = 0;

                max = startTime[chromosome[p]][tasksResource[r][chromosome[p]]] > max
                      ? startTime[chromosome[p]][tasksResource[r][chromosome[p]]] : max;
            }//若之前的工件占用的机器与当前nun工件j工序的相同，则将这些节点编号指向当前处理的节点i
        tasksResource[r][num] = t;
        startTime[num][t] = max + G[num][t].tmpTime;
        if (overhaul) {
            OVERHAULPTR tmp = overhaul[r];
            while (tmp) {
                /*
                if (tmp->startTime <= max && tmp->endTime > max ||
                    tmp->startTime >= max && tmp->startTime < startTime[num][t])
                    max = tmp->endTime;
                */
                if (tmp->startTime >= max && tmp->startTime < startTime[num][t]) {
                    int time = tmp->endTime - tmp->startTime;
                    tmp->startTime = startTime[num][t];
                    tmp->endTime = tmp->startTime + time;
                }
                if (max >= tmp->startTime && max < tmp->endTime)
                    max = tmp->endTime;
                tmp = tmp->nextOverhaul;
            }
        }
        startTime[num][t] = max + G[num][t].tmpTime;
    }

    int makespan = 0;
    for (int i = 0; i < jobNum; ++i)
        makespan = (startTime[i][machineNum - 1] > makespan) ? startTime[i][machineNum - 1] : makespan;

    if (mode) {
        machine = (MACHINEPTR *) malloc(machineNum * sizeof(MACHINEPTR));
        for (int mach = 0; mach < machineNum; ++mach) {
            MACHINEPTR node = machine[mach] = (MACHINEPTR) malloc(sizeof(struct machine)), tmp;

            for (int i = 0; i < len; ++i)
                for (int m = 0, flag = 1; m < jobNum && flag; ++m)
                    for (int n = 0; n < machineNum && flag; ++n)
                        if (G[m][n].point == i && G[m][n].machine == mach) {
                            node->job = m;
                            node->order = n + 1;
                            node->endTime = startTime[m][n];
                            node->startTime = startTime[m][n] - G[m][n].tmpTime;
                            tmp = node;
                            node = node->nextJob = (MACHINEPTR) malloc(sizeof(struct machine));
                            flag = 0;
                        }
            tmp->nextJob = NULL;
            free(node);
        }
    }

    return makespan;
}
