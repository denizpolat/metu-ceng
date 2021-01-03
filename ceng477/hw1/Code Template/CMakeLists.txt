cmake_minimum_required(VERSION 3.16)
project(Code_Template)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Code_Template
        parser.cpp
        parser.h
        ppm.cpp
        ppm.h
        raytracer.cpp
        tinyxml2.cpp
        tinyxml2.h)
