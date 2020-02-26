cmake_minimum_required(VERSION 3.15)

project(Slot_Signal)

set(CMAKE_CXX_STANDARD 20)

include_directories(./include/)

set(
        SOURCE
        ./include/base_function.hpp
        ./include/base_mutex.hpp
        ./include/base_observer.hpp
        ./include/slot_signal.hpp
)

add_executable(Slot_Signal ./src/main.cpp ${SOURCE})