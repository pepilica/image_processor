cmake_minimum_required(VERSION 3.17)
project(image_processor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall")

add_executable(image_processor
    image_processor.cpp
    src/image_manipulators.cpp src/image_manipulators.h
    src/matrix.h
    src/pixel.cpp src/pixel.h
    src/bitmap.cpp src/bitmap.h
    src/command_line_parser.cpp src/command_line_parser.h
    src/filter_pipeline_maker.cpp src/filter_pipeline_maker.h
    src/application.cpp src/application.h
    src/filter_pipeline.cpp src/filter_pipeline.h
    src/poly.h)

add_executable(tests
    tests/tests.cpp tests/tests.h
    src/image_manipulators.cpp src/image_manipulators.h
    src/matrix.h
    src/pixel.cpp src/pixel.h
    src/bitmap.cpp src/bitmap.h
    src/command_line_parser.cpp src/command_line_parser.h
    src/filter_pipeline_maker.cpp src/filter_pipeline_maker.h
    src/application.cpp src/application.h
    src/filter_pipeline.cpp src/filter_pipeline.h
    src/poly.h)
