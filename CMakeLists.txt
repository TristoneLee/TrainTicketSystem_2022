cmake_minimum_required(VERSION 3.16)
project(Ticket_System)

set(CMAKE_C_STANDARD 17)

include_directories(.src)

add_executable(Ticket_System
        ./src/MyString.h
        ./src/vector.hpp
        src/test.cpp
        src/Database.cpp
        src/Database.h)
