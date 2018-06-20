#include "jobshop.h"

void input_information_cb() {
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
