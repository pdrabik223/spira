cmake_minimum_required(VERSION 3.20)
project(spira)

set(CMAKE_CXX_STANDARD 17)


set(plotter_dir
        Plotter/plot.h
        Plotter/histogram.h
        Plotter/histogram.cpp

        Plotter/SubProcess.cpp
        Plotter/SubProcess.h
        Plotter/Error.cpp
        Plotter/Error.h)

include_directories(Plotter)

add_executable(spira
        spira-example.cpp
        ${plotter_dir})
