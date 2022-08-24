# MoTacToe_Solver
A simple automated solver for a version of Tic-Tac-Toe that has 3 players instead of 2.

## Description
This is a personal project I started after getting intrigued by a game called Tic-Tac-Moe, which is like Tic-Tac-Toe, but on a 5x5 grid and having 3 players instead of 2. (Yes, I did misremember the name while making the project.) It is meant to answer one question: if all three players are semi-competent Tic-Tac-Moe players- that is, they will always play a move that immediately wins for them, and will block a move that immediately wins for the next player- is it EVER possible for player 1 to win?

## Installing
This project uses CMake. If you do not have CMake, installation instructions can be found [here](https://cmake.org/install/).

After cloning the repository, create a build folder at the location of your choosing. From your terminal in your working directory, simply run `cmake -S <path_to_source_code> -B <path_to_build_folder>`. If no Makefile is generated, empty out the build folder, and run the same command as before, this time appended with the option `-G <your_makefile_generator>`. For more information on Makefile Generators, please consult [this page](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html).

After running CMake, navigate to your build folder and simply run `make`. If everything has been set up correctly, all executables should then compile successfully.

## Running
### IMPORTANT: This section will be updated as new executables are added.
Currently, The project only contains one executable: `test`, which tests the basic functionality of the Moe-Tac-Toe board. To run this program, simply navigate to your build folder via your command line, and call `./test`. If you are using Windows, run call `./test.exe` instead.
