cmake_minimum_required(VERSION 3.30)
project(Lab12SO)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lab12SO
        sender.c
        receiver.c
        methods.h
        methods.c)
