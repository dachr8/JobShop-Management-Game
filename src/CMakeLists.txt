cmake_minimum_required(VERSION 3.10)
project(JobShop_Management_Game C)

set(CMAKE_C_STANDARD 11)

add_executable(JobShop_Management_Game main.c struct.h io.c graph.c)