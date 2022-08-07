add_library(
    solver
    solver.cpp
    solver.hpp
)

target_include_directories(solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(solver PRIVATE game)