cmake_minimum_required(VERSION 3.16)
project(untitled1)

set(CMAKE_CXX_STANDARD 11)

add_executable(untitled1 main.cpp Particle.cpp Wall.cpp CurvyWall.cpp Cell.cpp Tissue.cpp MicroOrganism.cpp Circular.cpp Squadratic.cpp Triangular.cpp NucleoBase.cpp)