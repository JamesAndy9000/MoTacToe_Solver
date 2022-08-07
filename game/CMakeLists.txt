add_library(
    game
    mtt_board.hpp
    mtt_board.cpp
)

target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})