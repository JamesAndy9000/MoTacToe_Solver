#include <iostream>
#include "MTT_Board.h"

void printMenu();

void testEmptyBoard();
void testCustomPosition();
void testMoves();
void testUndo();
void testGameOver();

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
	int row, col;
	char winner;

	//Gather inputs from the user. Stop once the game ends.
	do
	{
		//Get the row and column from the user and make the move.
		std::cout << "Enter the array index for `row`: ";
		std::cin >> row;
		std::cin.ignore();

		std::cout << "Enter the array index for `col`: ";
		std::cin >> col;
		std::cin.ignore();

		if (board.makeMove(row, col))
		{
			//Show the board so far to make sure everything's right.
			std::cout << "\n Current board position: " << board.getBoardPosition() << "\n\n";
		}
		else
		{
			std::cout << "Move is invalid. Try again.\n";
		}
	}while (!(board.isOver()));

	winner = board.getWinner();
	//Once the game ends, make sure the board recognizes that it's a draw.
	if (winner == ' ')
		std::cout << "Game ended in a draw.\n\n";

	else
		std::cout << "Game ended in a victory for " << winner << ".\n\n";
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
