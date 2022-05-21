cmake_minimum_required(VERSION 3.22)
project(SimpleRegex)

set(CMAKE_CXX_STANDARD 20 -Wall -Wextra -pedantic)


add_executable(SimpleRegex main.cpp NotDeterminedMachine.h)
