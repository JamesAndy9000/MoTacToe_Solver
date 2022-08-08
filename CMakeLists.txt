cmake_minimum_required(VERSION 3.24.0)
project(MoTacToe_Solver VERSION 0.0.5)

add_subdirectory(game)

add_subdirectory(solver)

add_executable(test test.cpp)

target_link_libraries(test PRIVATE game solver)