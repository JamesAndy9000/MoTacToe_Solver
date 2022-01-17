#ifndef MTT_BOARD_H
#define MTT_BOARD_H
#include <string>
#include <array>
#include <unordered_set>
#include <stdexcept>
#include <cassert>
typedef std::size_t s_t;


const s_t NUM_PLAYERS = 3;
const s_t ROWS = 3;
const s_t COLUMNS = 5;
const s_t NUM_TO_WIN = 3;
enum Token{X = 'X', O = 'O', Y = 'Y', NONE = ' '};
const Token players[] = {X, O, Y, NONE};


/*Represents a specific square inside the game board, noted by its row and column.*/
struct Position
{
	s_t row;
	s_t col;
};


class MTT_Board
{
	private:
		/*Each element of this 2D array represents a square in the game board.
		 *The indexes for the rows start at 0 and increase from top to bottom.
		 *The indexes for the columns start at 0 and increase from left to right.
		 *Array access looks like "gameBoard[row][column]".
		 *As an example, accessing the top-left square looks like "gameBoard[0][0]".
		 *The state of each square is represented using each player's respective character.*/
		Token gameBoard[ROWS][COLUMNS];
		
		
		/*Represents the player whose turn it currently is.*/
		Token turnPlayer;
		
		
		//Variable representing whether the game has either been won by a player,
		//or ended in a draw.
		bool gameOver;
		
		
		/*Uses numerical values to record who won.
		 *Out of convenience, uses the same enum as the square states,
		 *Where "none" is used to represent a draw.
		 *Only really matters if "gameOver" is true.*/
		Token victor;
		
		
		/*Moves since the game started.
		 *Also used to help keep track of the turn player using modulo.*/
		s_t numberOfMoves;
		
		
		/*Returns a bool representing whether a target position
		 *is in bounds. Returns true if it is, returns false if it
		 *is not.*/
		bool boxInBounds(Position target) const;
		
		
		/*Helper function called by any function that can alter the board state,
		 *scpecifically by placing a symbol in the selected square.
		 *Function is to be called AFTER placing the symbol in the square.
		 *Thus, the given position will always be in bounds.
		 *Returns true iff the shape at the selected position creaes a 
		 *winning three-in-a-row sequence.*/
		bool isWinningMove(Position targetPos);
		
		
		/*Helper function called by isWinningMove().
		 *Draws a line emanating from the target position in a given direction
		 *(ie. horizontal, diagonal, vertical), */
		bool traceLine(Position targetPos, int rowIncrease, int colIncrease) const;
		
		
		/*Helper function to handle repeated logic in checkLine().
		 *Checks the selected square to make sure it matches the target symbol,
		 *and determines wheteher or not the loop should continue.*/
		bool checkLineForMatch(Position checkPos, char targetSymbol,
							 int& numInARow, bool& isWinning) const;
		
		
		/*Returns a copy of the symbol indicated at the specified position.
		 *Created for use in const methods.*/
		Token getToken(Position position) const
		{
			return gameBoard[position.row][position.col];
		}
		
		
		/*Helper function called by any method that can alter the game board.
		 *Responsible for actually placing the symbol on the correct spot on the board.
		 *Precondition: supplied position is within bounds.*/
		void placeToken(Position position, char token)
		{
			gameBoard[position.row][position.col] = static_cast<Token> (token);
		}
		
		
		/*Helper function which is called by any method which sets up from a provided board position.
		 *Starting at `position`, places a number of consecutive empty spaces on the board,
		 *equal to the number represented by `spaces`.
		 *Relies on calling function to check whether array has enough space.*/
		void placeSpaces(Position position, s_t spaces);
	
	
	public:
		//Creates an empty Moe-Tac-Toe board.
		//3 row-by-5 column grid containing all empty spaces.
		MTT_Board();


		/*Creates a Moe-Tac-Toe board with some moves already made.
		 *Notation for "boardPosition" is similar to Forsyth-Edwards Notation for Chess positions;
		 *Each string under this notation contains two fields, each separated by a space.
		 *The first field describes where each shape has been drawn.
		 *Starting from the top row and ending on the bottom, each occupied squre has its contents
		 *shown from left to right.
		 *The contents of each square is shown as either an X, O, or Y,
		 *representing players 1, 2, and 3 respectively.
		 *Similar to FEN, empty squares are noted by the number of consecutive empty squares, ([1-5]),
		 *and each row is separated by a "/".
		 *The next field simply shows who the turn player is, that being either X, O, or Y.
		 *Sets the "gameOver" bit if the supplied position depicts either a player victory or a draw.
		 *This constructor will throw an Invalid_Position_Exception if the input string
		 *has invalid formatting, or ends up describing a board which does not fit into a 3x5 grid.
		 *This constructor does not check if the position is actually attainable in a real game without skipping turns,
		 *but it does throw an exception if there are multiple winners.*/
		MTT_Board(const std::string boardPosition);


		/*Places an X, O, or Y in the desired square,
		 *depending on current turn player.
		 *Arguments are accepted based on storage coordinates,
		 *ie. [0 - (ROWS-1)] and [0 - (COLUMNS - 1)], respectively.
		 *Returns false iff the move is out of bounds,
		 *or the target square is already occupied*/
		bool makeMove(s_t row, s_t column);
		
		
		/*Returns true iff either the game has gone on for the maximum possible number of turns,
		 *or there is a line of three consecutive spaces occupied by the same player.*/
		bool isOver() const { return gameOver; }
		
		
		/*Returns an ID representing the victor. If the game has not yet been won,
		 *or ended in a draw, returns "NONE".*/
		Token getWinner() const { return victor; }
		
		
		//COMMENT OUT IF NOT TESTING.
		s_t getNumMoves() const { return numberOfMoves; }
		
		
		/*Returns a string describing the current board position,
		 *using the same notation as the boardPosition Constructor.*/
		std::string getBoardPosition() const;
		
		
		/*Erases the symbol in the target position, decreases the turn counter,
		 *and reverts the turn player back to the previous player.
		 *Function is successful iff the symbol on the target position matches the
		 *previous turn player, and the position is in bounds.
		 *Function returns true if successful, and false if not.*/
		bool undoMove(s_t row, s_t col);
};


#endif
