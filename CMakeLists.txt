cmake_minimum_required(VERSION 3.0)

project(einstein)

set(CMAKE_CXX_STANDARD 11)

add_executable(
    einstein
    ABSolver.cpp
    ABSolver.h
    Board.cpp
    Board.h
    BoardUtil.cpp
    BoardUtil.h
    Constants.h
    INCLUDE.h
    main.cpp
    MCTSSolver.cpp
    MCTSSolver.h
    MyRand.cpp
    MyRand.h
    Simulator.cpp
    Simulator.h
    Utils.cpp
    Utils.h
)

