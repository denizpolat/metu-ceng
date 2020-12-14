cmake_minimum_required(VERSION 3.16)
project(hw2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(hw2
        sample_main_files/main_for_jpeg_to_ppm.cpp
        sample_main_files/main_for_matrix_inversion.cpp
        sample_main_files/main_for_write_jpeg_file.cpp
        camera.cpp
        camera.h
        ctpl_stl.h
        hit_record.h
        image.cpp
        image.h
        integrator.cpp
        integrator.h
        jpeg.cpp
        jpeg.h
        main.cpp
        Makefile
        material.cpp
        material.h
        matrix.cpp
        matrix.h
        mesh.cpp
        mesh.h
        parser.cpp
        parser.h
        point_light.cpp
        point_light.h
        ppm.cpp
        ppm.h
        ray.cpp
        ray.h
        rotation.cpp
        rotation.h
        scaling.cpp
        scaling.h
        scene.cpp
        scene.h
        sphere.cpp
        sphere.h
        texture.cpp
        texture.h
        timer.cpp
        timer.h
        tinyxml2.cpp
        tinyxml2.h
        translation.cpp
        translation.h
        triangle.cpp
        triangle.h
        triangular.h
        utility.h
        vector2f.h
        vector3f.h
        vector4f.h transformation.cpp transformation.h)
