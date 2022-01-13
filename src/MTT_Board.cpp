#include "MTT_Board.h"


//Public Functions

//Default constructor.
MTT_Board::MTT_Board()
{
	Position position;

	//Initialize all of the blocks in the array to contain "NONE".
	for (position.row = 0; position.row < ROWS; position.row++)
	{
		for (position.col = 0; position.col < COLUMNS; position.col++)
		{
			placeToken(position, NONE);
		}
	}

	//Set the rest of the values and flags to the value they have at the start of the game.
	turnPlayer = X;
	gameOver = false;
	victor = NONE;
	numberOfMoves = 0;
}


//Parameterized constructor.
MTT_Board::MTT_Board(std::string boardPosition)
{
	numberOfMoves = 0;
	gameOver = false;
	enum State { FILL_BOARD, GET_TURN };
	State curState = FILL_BOARD;
	Position curPos{0, 0};
	bool positionComplete = false;
	s_t squaresFilled = 0;
	s_t totalSquares = 0;
	s_t consecutiveBlanks = 0;
	char curChar;

	//Analyze the portion of the string which depicts the placement of tokens on the board.*/
	for (s_t index = 0; index < boardPosition.size(); index++)
	{
		curChar = boardPosition[index];

		if (positionComplete)
		{
			throw std::invalid_argument("Unexpected characters after Turn Player token.");
		}
		else
		{
			switch (curChar)
			{

				case 'X':
				case 'O':
				case 'Y':
					switch (curState)
					{
						/*In this case, the character represents a token,
						 *make sure curPos is in bounds.
						 *If it is, check to make sure the row has enough space for unplaced blanks and the current token.
						 *If it does, place said spaces and the token on the board in the correct positions,
						 *and prepare the loop to move on to the next column.
						 *If not, throw an exception.*/
						case FILL_BOARD:
							if ((squaresFilled + consecutiveBlanks + 1) <= COLUMNS)
							{
								/*Place any spaces that may be there,
								 *and update local variables to reflect this.*/
								placeSpaces(curPos, consecutiveBlanks);
								squaresFilled += consecutiveBlanks;
								totalSquares += consecutiveBlanks;
								curPos.col += consecutiveBlanks;

								/*Place the indicated token on the board,
								 *updating object and local variables.*/
								placeToken(curPos, curChar);
								curPos.col++;
								numberOfMoves++;
								squaresFilled++;
								totalSquares++;
								consecutiveBlanks = 0;
							}
							else
							{
								throw std::invalid_argument("Invalid position; Indicated tokens do not fit on row.");
							}
							break;

						//Set the turn player, and set the positionComplete flag.
						case GET_TURN:
							turnPlayer = static_cast<Token>(curChar);
							positionComplete = true;
							break;

						default:
							throw std::logic_error("This shouldn't be possible.");
					}
					break;

				/*In the FILL_BOARD state, update the number of blanks.
				 *In all other states, throw an exception.*/
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					switch (curState)
					{
						case FILL_BOARD:
							consecutiveBlanks *= 10;
							consecutiveBlanks += (curChar-48);	//Convert from a character representing a digit to the number.
							break;

						default:
							throw std::invalid_argument("Invalid string; integer character in unexpected location.");
					}
					break;

				/*In the FILL_BOARD state, first make sure all squares in the row have either all been filled,
				 *or been shown to be blank.
				 *Next, make sure more rows remain.
				 *If they do, fill in any unmarked blanks and move on to the next row.
				 *If not, or if not in the FILL_BOARD state, throw an exception.*/
				case '/':
					switch (curState)
					{
						case FILL_BOARD:
							//I subtract 1 from ROWS because I'm comparing it to an index.
							if ((squaresFilled+consecutiveBlanks) == COLUMNS && curPos.row < (ROWS-1))
							{
								/*Place any blanks that may be there.*/
								placeSpaces(curPos, consecutiveBlanks);
								squaresFilled += consecutiveBlanks;
								totalSquares += consecutiveBlanks;
								consecutiveBlanks = 0;

								//Move on to the next row.
								curPos.row++;
								curPos.col = 0;
								squaresFilled = 0;
							}
							else
							{
								throw std::invalid_argument("Invalid string; not all squares in row accounted for.");
							}
							break;

						default:
							throw std::invalid_argument("Invalid string; slash character in unexpected location.");
					}
					break;

				/*In FILL_BOARD state, make sure any unplaced spaces will completely fill the board.
				 *If they do, place said spaces, and change curState to GET_TURN.
				 *If not, throw an exception.
				 *In any other state, throw an exception.*/
				case ' ':
					switch (curState)
					{
						case FILL_BOARD:
							if (totalSquares + consecutiveBlanks == (ROWS*COLUMNS))
							{
								placeSpaces(curPos, consecutiveBlanks);
								curState = GET_TURN;
							}
							else
							{
								throw std::invalid_argument("Invalid position string; not all squares on row are accounted for.");
							}
							break;

						default:
							throw std::invalid_argument("Invalid position string; space character in unexpected location.");
					}
					break;

				/*If the character is anything else, throw an exception.*/
				default:
					throw std::invalid_argument("Invalid position string; illegal character detected.");
					break;
			}
		}
	}
}


/*After verifying the desired move is in bounds and empty, places the turn player's token there.
 *Returns true iff the token was placed successfully.*/
bool MTT_Board::makeMove(s_t row, s_t column)
{
	bool successfulMove;

	/*Game's over, can't play anymore.*/
	if (gameOver)
	{
		successfulMove = false;
	}

	else
	{
		Position position {row, column};
		successfulMove = (boxInBounds(position) && getToken(position) == NONE);

		//If the above check failed, then the move is invalid. No need to advance the game state.
		if (successfulMove)
		{
			//Increase the move count, then actually place the token.
			numberOfMoves++;
			placeToken(position, turnPlayer);

			/*Check if the turn player won with that last move.*/
			if (isWinningMove(position))
			{
				gameOver = true;
				victor = turnPlayer;
			}

			/*Check whether the game is a draw, by comparing it to the de-facto move limit,
			 *which is `ROWS` multiplied by `COLUMNS`.
			 *No need to set "victor", because it will either be `NONE`,
			 *or whoever won with this move.*/
			if (numberOfMoves == (ROWS * COLUMNS))
			{
				gameOver = true;
			}

			/*Now that the hypothetical winner has been determined,
			 *we can advance the turn player.*/
			s_t playerIndex = (numberOfMoves) % NUM_PLAYERS;
			turnPlayer = players[playerIndex];
		}
	}
	return successfulMove;
}


/*Returns a string representation of the board state.
 *Scrolls through each row on the board, depicting the contents of each square,
 *and the number of empty spaces inbetween.*/
std::string MTT_Board::getBoardPosition() const
{
	Position curSquare;
	std::string boardPosition = "";
	int numberOfBlanks;
	Token token;

	for (curSquare.row = 0; curSquare.row < ROWS; curSquare.row++)
	{
		/*Bulk of the algorithm; figure out where all of the tokens are.
		 *If the box is empty, add to the counter of empty spaces.
		 *If square is not empty, add the previous number of empty squares, (if not 0),
		 *and the contents of the square to the position string.*/
		numberOfBlanks = 0;
		for (curSquare.col = 0; curSquare.col < COLUMNS; curSquare.col++)
		{
			token = getToken(curSquare);
			switch (token)
			{
				//Non-empty square; print the contents, as well as the number of blanks before it.
				case X:
				case O:
				case Y:
					//If `numberOfBlanks` is 0, then the last square wasn't empty.
					//In that case, no need to append the number of preceding blanks.
					if (numberOfBlanks != 0)
					{
						boardPosition += std::to_string(numberOfBlanks);
						numberOfBlanks = 0;
					}

					//Now grab the appropriate character for the token, and append it to the string.
					boardPosition += token;
					break;

				//Empty square, Don't add anythig to the string, unless we're on the last column.
				case NONE:
					numberOfBlanks++;

					//Special case where the last square on the row hasn't been filled yet.
					if (curSquare.col == COLUMNS-1)
					{
						boardPosition += std::to_string(numberOfBlanks);
					}
					break;

				default:
					throw std::logic_error("Someone drew something weird on this board.");
					break;
			}
		}

		/*Separate each row by a slash. No need to do it on the last row*/
		if (!(curSquare.row == (ROWS-1)))
		{
			boardPosition += '/';
		}
	}

	/*Lastly, Append the turn player to the board position string.
	 *Make sure it is separated by a space from the token section.*/
	boardPosition += ' ';
	boardPosition += turnPlayer;

	return boardPosition;
}


bool MTT_Board::undoMove(s_t row, s_t col)
{
	Position target = {row, col};
	if (!(this->boxInBounds(target)))
	{
		return false;
	}

	//Use modulo stuff to determine the previous player.
	s_t playerIndex = (numberOfMoves-1) % NUM_PLAYERS;
	Token prevPlayer = players[playerIndex];

	if (prevPlayer != getToken(target))
	{
		return false;
	}

	//At this point, the function is successful.
	this->placeToken(target, ' ');
	numberOfMoves--;
	turnPlayer = prevPlayer;

	/*I had some difficulty deciding whether to do this,
	 *but then I realized that the way I was using this,
	 *undoMove would never be used on a won board to undo a move
	 *that wasn't winning.*/
	gameOver = false;

	//Function is always successful when reaching this point, so
	return true;
}


//Private functions
//-------------------------------------------------------------------------------------------------
bool MTT_Board::boxInBounds(Position target) const
{
	bool goodRow = (target.row >= 0) && (target.row < ROWS);
	bool goodCol = (target.col >= 0) && (target.col < COLUMNS);

	return (goodRow && goodCol);
}

bool MTT_Board::isWinningMove(Position targetPos)
{
	assert(boxInBounds(targetPos));


	bool verticalWin = traceLine(targetPos, 1, 0);		//Vertical; row changes, column doesn't.
	bool horizontalWin = traceLine(targetPos, 0, 1);	//Horizontal; Column changes, row doesn't.
	bool upDiagWin = traceLine(targetPos, -1, 1);		//Down and to the right.
	bool downDiagWin = traceLine(targetPos, 1, 1);		//Up and to the right.

	return (verticalWin || horizontalWin || upDiagWin || downDiagWin);
}


/*Searches for N-in-a-row pattern by "drawing" 2 lines
 *emanating from the target square in opposite directions
 *and scanning them for matching symbols.
 *Lines stop emanating once a non-matching symbol is found, or line goes out of bounds.*/
bool MTT_Board::traceLine(Position targetPos, int rowIncrease, int colIncrease) const
{
	bool result = false;			//Flag for whether or not a complete line has been found
									//Start by assuming it isn't.

	int rowDistance, colDistance;	//Distance between checked square and target.
	Position checkPos;				//Coordinates for the current square to be checked.
	int numInARow = 1;

	//flags for checking in either direction.
	bool checkForwardLine = true;
	bool checkBackwardLine = true;

	Token targetSymbol = getToken(targetPos);	//Symbol each square will be compared to.

	/*Check each square above the target, until either a square is found not matching the target,
	 *search goes out of bounds, or distance from target becomes so great as to not matter.
	 *Exit function early if the number of matches found equals a win.*/
	for (rowDistance = rowIncrease, colDistance = colIncrease;
		 (rowDistance < NUM_TO_WIN) && (colDistance < NUM_TO_WIN);
		 rowDistance+=rowIncrease, colDistance+=colIncrease)
	{
		/*If both lines have been cut, then there's no need to continue checking.*/
		if (!(checkForwardLine || checkBackwardLine))
		{
			result = false;
			break;
		}

		/*Check the next square in one direction.
		 *Will not execute if line has gone out of bounds, or encountered non-matching symbol.*/
		if (checkForwardLine)
		{
			checkPos.row = targetPos.row + rowDistance;
			checkPos.col = targetPos.col + colDistance;

			checkForwardLine = checkLineForMatch(checkPos, targetSymbol,
				numInARow, result);
			if (result)
			{
				break;
			}
		}

		//Perform the same operations in the other direction.
		if (checkBackwardLine)
		{
			checkPos.row = targetPos.row - rowDistance;
			checkPos.col = targetPos.col - colDistance;

			checkBackwardLine = checkLineForMatch(checkPos, targetSymbol,
												numInARow, result);
			if (result)
			{
				break;
			}
		}
	}

	return result;
}


/**/
bool MTT_Board::checkLineForMatch(Position checkPos, char targetSymbol,
								int& numInARow, bool& isWinning) const
{
	if (boxInBounds(checkPos)
		&& (getToken(checkPos) == targetSymbol))
	{
		numInARow++;
		if(numInARow == NUM_TO_WIN)
		{
			isWinning = true;
		}
		return true;
	}
	else
	{
		return false;
	}
}


void MTT_Board::placeSpaces(Position position, s_t spaces)
{
	for (s_t space = 0; space < spaces; space++)
	{
		placeToken(position, NONE);
		position.col++;
	}
}
