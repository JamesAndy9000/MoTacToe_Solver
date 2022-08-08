#include <iostream>
#include "mtt_board.hpp"

void printMenu();

void testEmptyBoard();
void testCustomPosition();
void testMoves();
void testUndo();
void testGameOver();
void userFinishesGame(MTT_Board& board);


int main(int argc, char** argv)
{
	char menuChoice;
	do
	{
		//Get menu choice and execute appropriate function.
		printMenu();
		menuChoice = std::cin.get();
		std::cin.ignore();

		switch (std::toupper(menuChoice))
		{
			case 'A':
				testEmptyBoard();
				break;

			case 'B':
				testCustomPosition();
				break;

			case 'C':
				testMoves();
				break;

			case 'D':
				testGameOver();
				break;

			case 'U':
				testUndo();
				break;

			case 'Q':
				std::cout << "Goodbye.\n";
				break;

			default:
				std::cout << "Invalid choice. Please try again.\n\n";
		}
	} while (std::toupper(menuChoice) != 'Q');
	return 0;
}

void printMenu()
{
	std::cout << "Choose one of the following:\n";
	std::cout << "A: Ensure empty board initializes correctly.\n";
	std::cout << "B: Ensure board with custom position initializes correctly.\n";
	std::cout << "C: Ensure moves are played correctly.\n";
	std::cout << "D: Ensure endgames are handled correctly.\n";
	std::cout << "U: Ensure moves are undone correctly.\n";
	std::cout << "Q: Quit.\n";
}


//Simply run the blank constructor and ensure everything is set up correctly.
void testEmptyBoard()
{
	MTT_Board board = MTT_Board();

	//Check board and turn player.
	std::cout << "Board position after initialization: " << board.getBoardPosition() << std::endl;

	//Make sure `gameOver` bit is set correctly.
	std::cout << (board.isOver() ? "`gameOver` is not set correctly.\n" : "`gameOver is set correctly.\n");

	//Make sure `victor` starts off set to NONE.
	std::cout << ((board.getWinner() == ' ') ? "`victor` is set correctly.\n" : "`victor` is not set correctly.\n");

	std::cout << std::endl;
}


void testCustomPosition()
{
	MTT_Board board;
	std::string testPosition;

	testPosition = "X4/5/5 O";
	board = MTT_Board(testPosition);

	std::cout << (board.getBoardPosition() == testPosition
			? "Position set correctly." : "Something went wrong.") << std::endl;
}


void testMoves()
{
	MTT_Board board = MTT_Board();

	std::cout << "Placing 'X' in top-left corner...";
	std::cout << (board.makeMove(0, 0) ? "Done.\n" : "Something went wrong.\n");
	std::cout << "Position should be X4/5/5 O\n";
	std::cout << "Position is:       " << board.getBoardPosition() << std::endl;

	std::cout << "Placing 'O' in middle square...";
	std::cout << (board.makeMove(1, 2) ? "Done.\n" : "Something went wrong.\n");
	std::cout << "Position should be X4/2O2/5 Y\n";
	std::cout << "Position is:       " << board.getBoardPosition() << std::endl;

	std::cout << "Placing 'Y' in bottom-right corner...";
	std::cout << (board.makeMove(2, 4) ? "Done.\n" : "Something went wrong.\n");
	std::cout << "Position should be X4/2O2/4Y X\n";
	std::cout << "Position is:       " << board.getBoardPosition() << std::endl;

	std::cout << "Trying to place 'X' in bottom right corner. This should fail...";
	std::cout << (board.makeMove(2, 4) ? "It didn't fail; something's wrong.\n" : "Done.\n");
	std::cout << "Position should be X4/2O2/4Y X\n";
	std::cout << "Position is:       " << board.getBoardPosition() << std::endl;

	std::cout <<"Placing 'X' in far-left center square...";
	std::cout << (board.makeMove(1, 0) ? "Done.\n" : "Something went wrong.\n");

	std::cout << "Final board position should be: X4/X1O2/4Y O\n";
	std::cout << "Final board position is:        " << board.getBoardPosition() << std::endl;

	std::cout << "Press \"Enter\" to continue...\n";
	std::cin.get();
}


/*Input a sequence of moves which should result in a draw.
 *Test after each move to make sure it does end in a draw.*/
void testGameOver()
{
	MTT_Board board = MTT_Board();

	auto isWon = [&board](s_t row, s_t col)
	{
		std::cout << "Old: " << board.getBoardPosition() << "\n";
		board.makeMove(row, col);

		std::cout << "New: " << board.getBoardPosition() << "\n";

		std::cout << (board.getWinner() == 'X'
				? "Test passed.\n\n"
				: "Test failed.\n\n");
	};

	//Vertical wins
	std::cout << "Vertical wins...\n";

	//Winner on top
	std::cout << "Winner on top:\n";
	board.setBoard("1OY2/XOY2/X4 X");
	isWon(0, 0);

	//Winner in middle
	std::cout << "Winner in middle:\n";
	board.setBoard("XOY2/1OY2/X4 X");
	isWon(1, 0);
	
	//Winner on bottom
	std::cout << "Winner on bottom:\n";
	board.setBoard("XOY2/XOY2/5 X");
	isWon(2, 0);
	std::cout << "\n";
	

	//Horizontal wins
	std::cout << "Horizontal wins...\n";

	//Winner on left
	std::cout << "Winner on left:\n";
	board.setBoard("1XX2/1OO2/1YY2 X");
	isWon(0, 0);

	//Winner in middle
	std::cout << "Winner in middle:\n";
	board.setBoard("X1X2/O1O2/Y1Y2 X");
	isWon(0, 1);

	//Winner on right
	std::cout << "Winner on right:\n";
	board.setBoard("XX3/OO3/YY3 X");
	isWon(0, 2);
	std::cout << "\n";

	//Diagonal wins
	std::cout << "Diagonal wins...\n";

	//Winner top-left
	std::cout << "Winner top-left:\n";
	board.setBoard("5/1X3/2X2 X");
	isWon(0, 0);

	//Winner top-right
	std::cout << "Winner top-right:\n";
	board.setBoard("5/1X3/X4 X");
	isWon(0, 2);

	//Winner bottom-left
	std::cout << "Winner bottom-left:\n";
	board.setBoard("2X2/1X3/5 X");
	isWon(2, 0);

	//Winner bottom-right
	std::cout << "Winner bottom-right:\n";
	board.setBoard("X4/1X3/5 X");
	isWon(2, 2);
	
	//Winner center
	std::cout << "Winner center:\n";
	board.setBoard("X4/5/2X2 X");
	isWon(1, 1);
}


void testUndo()
{
	std::cout << "\nPlease make sure makeMove() has been tested.\n\n";

	std::cout << "Undoing a single move...\n";

	auto board = MTT_Board();
	board.makeMove(0, 0);
	board.undoMove(0, 0);

	std::cout << "Position should be: 5/5/5 X\n";
	std::cout << "Position is:      : " << board.getBoardPosition() << "\n";
	std::cout << "numberOfMoves should be 0 and is " << board.getNumMoves() << "\n\n";

	std::cout << "Undoing a winning move...\n";

	board = MTT_Board("XOY2/XOY2/5 X");
	board.makeMove(2, 0);
	board.undoMove(2, 0);

	std::cout << "Position should be: XOY2/XOY2/5 X\n";
	std::cout << "Position is:        " << board.getBoardPosition() << "\n";
	std::cout << "numberOfMoves should be 6 and is " << board.getNumMoves() << "\n";
	std::cout << "gameOver should be false and is " << board.isOver() << "\n\n";
}
