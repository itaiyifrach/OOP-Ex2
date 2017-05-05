#include "initialize.h"

int initialize(int argc, char** argv, char** board, int numRows, int numCols, string& attackApath, string& attackBpath) {
	string basePath, boardPath, popenResult;
    
	//if didn't get argument- path is the cwd, else it is the path given in argv[1]
	if (argc <= 1)
	{
		char* temp = _getcwd(NULL, 256);	
		basePath = temp;
		free(temp);
	}
		
	else
		basePath = argv[1];
	//check if path is valid- if not print error and return -1
	if (extractFilesFromPath(basePath,popenResult) != 0)
	{
		cout << WRONG_PATH << basePath << endl;
		return -1;
	}
	//check is a flag var- equals -1 if there was a problem with one of the files
	int check = 0;
	//check if there was no board file-if so print error and flag check.
	if (parsePath(basePath,popenResult, ".sboard", boardPath) != 0)
	{
		cout << MISSING_BOARD << basePath << endl;
		check = -1;
	}

	//check if there was no attackA file-if so print error and flag check.
	if (parsePath(basePath,popenResult, ".attack-a", attackApath) != 0)
	{
		cout << MISSING_ATTACK_FILE_A << basePath << endl;
		check = -1;
	}

	//check if there was no attackB file-if so print error and flag check.
	if (parsePath(basePath,popenResult, ".attack-b", attackBpath) != 0)
	{
		cout << MISSING_ATTACK_FILE_B << basePath << endl;
		check = -1;
	}
	
	if (check == -1)
		return-1;

	// \n and \r\n compatible.
	// read getline(inf) X 10, getc from line X 10. empty line / invalid char is a space.
	parseBoard(boardPath, board,numRows,numCols);

	// check if the board is valid
	int mistakes[7] = { 0 };
	char shipMistakeTypeA = 0, shipMistakeTypeB = 0;
	if (checkBoard(board, mistakes) == false)
	{
		for (int i = 0; i < 7; i++)
		{
			if (mistakes[i] != 0)
			{
				switch (i)
				{
				case 0:
					shipMistakeTypeA = mistakes[0];
					cout << BOARD_MISTAKE_0 << shipMistakeTypeA << FOR_PLAYER << "A" << endl;
					break;
				case 1:
					shipMistakeTypeB = mistakes[1];
					cout << BOARD_MISTAKE_0 << shipMistakeTypeB << FOR_PLAYER << "B" << endl;
					break;
				case 2:
					cout << BOARD_MISTAKE_2 << endl;
					break;
				case 3:
					cout << BOARD_MISTAKE_3 << endl;
					break;
				case 4:
					cout << BOARD_MISTAKE_4 << endl;
					break;
				case 5:
					cout << BOARD_MISTAKE_5 << endl;
					break;
				case 6:
					cout << BOARD_MISTAKE_6 << endl;
					break;
				default:
					break;
				}
			}
		}
		return -1;
	}

	//if all is well return 0
	return 0;
}


int extractFilesFromPath(const string& path, string& popenResult)
{
	string newPath = "\"" + path + "\"";
	//system command to find all files from current dir.
	string cmd1 = "2>NUL dir " + newPath + " /b /a-d";	
	FILE* fp = _popen(cmd1.c_str(), "r");
	if (fp == NULL)
		return -1;
	char buffer[4096];
	while (fgets(buffer, 4095, fp))
	{
		popenResult += string(buffer);
	}	
	return 0;
}

int parsePath(const string& basePath, string& poppenResult, const string& file_suffix, string& file_path)
{
	std::istringstream strStream(poppenResult);
	string line;
	while(getline(strStream,line))
	{
		if(line.find(file_suffix, line.size() - file_suffix.size()) !=string::npos)
		{
			file_path = basePath + "/" + line;
			return 0;
		}
	}
	
	return -1;
}

void parseBoard(const string& boardPath,char**& board,int rows,int cols)
{
	ifstream ifs(boardPath);
	string line;
	for (int i = 0; i < rows; i++)
	{		
		getline(ifs, line);		
		size_t j = 0;
		for (j; j < line.size(); j++)
		{
			//check if char is valid
			if (line[j] == 'b' || line[j] == 'B' || line[j] == 'p' || line[j] == 'P' ||
				line[j] == 'm' || line[j] == 'M' || line[j] == 'd' || line[j] == 'D')
				board[i][j] = line[j];
			else
				board[i][j] = ' ';
		}
		//line is less than 10 characters-add spaces
		for(j;j<10;j++)
			board[i][j] = ' ';
	}
}



/*	mistakes:
*	[0] = Wrong size or shape for ship <char> for player A
*	[1] = Wrong size or shape for ship <char> for player B
*	[2] = Too many ships for player A
*	[3] = Too few ships for player A
*	[4] = Too many ships for player B
*	[5] = Too few ships for player B
*	[6] = Adjacent Ships on Board
*
*/
bool checkBoard(char** board, int* mistakes)
{
	char** markedBoard = new char*[ROWS_WIDTH];
	for (int i = 0; i < COLS_WIDTH; ++i)
		markedBoard[i] = new char[COLS_WIDTH];
	// copy board to markedBoard
	for (int i = 0; i < ROWS_WIDTH; i++)
		for (int j = 0; j < COLS_WIDTH; j++)
			markedBoard[i][j] = board[i][j];

	bool isValid = true, validShape = false;
	int validShipsA = 0, validShipsB = 0; // counter for players A/B valid ships found on board
	char currCharChecked = ' ';
	int currPlayer = 0;

	// scanning the board
	for (int i = 0; i < ROWS_WIDTH; i++)
	{
		for (int j = 0; j < COLS_WIDTH; j++)
		{
			currCharChecked = markedBoard[i][j];
			switch (currCharChecked)
			{
				// player A options:
			case 'B':
				validShape = checkShape(board, markedBoard, i, j, currCharChecked, SIZE_SHIP_B, mistakes, 0);
				currPlayer = 0;
				break;
			case 'P':
				validShape = checkShape(board, markedBoard, i, j, currCharChecked, SIZE_SHIP_P, mistakes, 0);
				currPlayer = 0;
				break;
			case 'M':
				validShape = checkShape(board, markedBoard, i, j, currCharChecked, SIZE_SHIP_M, mistakes, 0);
				currPlayer = 0;
				break;
			case 'D':
				validShape = checkShape(board, markedBoard, i, j, currCharChecked, SIZE_SHIP_D, mistakes, 0);
				currPlayer = 0;
				break;
				// player B options:
			case 'b':
				validShape = checkShape(board, markedBoard, i, j, currCharChecked, SIZE_SHIP_B, mistakes, 1);
				currPlayer = 1;
				break;
			case 'p':
				validShape = checkShape(board, markedBoard, i, j, currCharChecked, SIZE_SHIP_P, mistakes, 1);
				currPlayer = 1;
				break;
			case 'm':
				validShape = checkShape(board, markedBoard, i, j, currCharChecked, SIZE_SHIP_M, mistakes, 1);
				currPlayer = 1;
				break;
			case 'd':
				validShape = checkShape(board, markedBoard, i, j, currCharChecked, SIZE_SHIP_D, mistakes, 1);
				currPlayer = 1;
				break;
			default:
				continue;
			}

			if (validShape == true)
			{
				if (currPlayer == 0)
					validShipsA++;
				else
					validShipsB++;
			}
		}
	}
	// finish scanning the board

	// checking number of valid ships of players A and B:
	if (validShipsA > SHIPS_NUM)
		mistakes[2] = 1;
	else if (validShipsA < SHIPS_NUM)
		mistakes[3] = 1;
	if (validShipsB > SHIPS_NUM)
		mistakes[4] = 1;
	else if (validShipsB < SHIPS_NUM)
		mistakes[5] = 1;

	// checking if there were any mistakes
	for (int i = 0; i < 7; i++)
		if (mistakes[i] != 0)
			isValid = false;

	delete[](markedBoard);
	return isValid;
}

bool checkShape(char** board, char** markedBoard, int posI, int posJ, char shipType, int shipSize, int* mistakes, int player)
{
	int countTiles = 0, possibleAdj = 0, special = false;
	bool checkValid = true;
	int limit = min(posJ + shipSize, COLS_WIDTH);

	// check horizontal shape
	for (int j = posJ; j < limit; j++)
	{
		// checking upper bound
		if (posI != 0)	// not an edge
		{
			if (checkBound(board, shipType, posI - 1, j, mistakes, player, &possibleAdj))
				checkValid = false;
		}
		// checking lower bound
		if (posI != ROWS_WIDTH - 1)	// not an edge
		{
			if (j == posJ)	// special case !!!
			{
				if (player == 0 && mistakes[0] == 0)
				{
					if (checkBound(board, shipType, posI + 1, j, mistakes, player, &possibleAdj)) {
						checkValid = false;
						special = true;
					}
				}
				if (player == 1 && mistakes[1] == 0)
				{
					if (checkBound(board, shipType, posI + 1, j, mistakes, player, &possibleAdj)) {
						checkValid = false;
						special = true;
					}
				}
			}
			else
			{
				if (checkBound(board, shipType, posI + 1, j, mistakes, player, &possibleAdj))
					checkValid = false;
			}
		}
		if (j == posJ)	// need to check the back also
		{
			if (j != 0)	// not an edge, checking the back
			{
				if (checkBound(board, shipType, posI, j - 1, mistakes, player, &possibleAdj))
					checkValid = false;
			}
		}
		if (j == posJ + shipSize - 1)	// need to check the next also
		{
			if (j != COLS_WIDTH - 1)	// not an edge, checking the next
			{
				if (checkBound(board, shipType, posI, j + 1, mistakes, player, &possibleAdj))
					checkValid = false;
			}
		}

		if (board[posI][j] == shipType)
		{
			markedBoard[posI][j] = 'X'; // marking the curr tile
			countTiles++;				// counting fitted tiles
		}
		else break;		// if any tile isn't as the checked one
	} // end of for
	if (countTiles == shipSize)		// if marked the full ship size
	{
		if (checkValid)
		{
			if (possibleAdj == 1)
				mistakes[6] = 1;
			return true;
		}
	}
	// end of horizontal check

	// check vertical shape
	countTiles = 0, possibleAdj = 0;
	checkValid = true;
	limit = min(posI + shipSize, ROWS_WIDTH);

	for (int i = posI; i < limit; i++)
	{
		// checking left bound
		if (posJ != 0)	// not an edge
		{
			if (checkBound(board, shipType, i, posJ - 1, mistakes, player, &possibleAdj))
				checkValid = false;
		}
		// checking right bound
		if (posJ != COLS_WIDTH - 1)	// not an edge
		{
			if (checkBound(board, shipType, i, posJ + 1, mistakes, player, &possibleAdj))
				checkValid = false;
		}
		if (i == posI)	// need to check the up also
		{
			if (i != 0)	// not an edge, checking the up
			{
				if (checkBound(board, shipType, i - 1, posJ, mistakes, player, &possibleAdj))
					checkValid = false;
			}
		}
		if (i == posI + shipSize - 1)	// need to check the down also
		{
			if (i != ROWS_WIDTH - 1)	// not an edge, checking the down
			{
				if (checkBound(board, shipType, i + 1, posJ, mistakes, player, &possibleAdj))
					checkValid = false;
			}
		}

		if (board[i][posJ] == shipType)
		{
			if (special)
			{
				if (player == 0)	// player A
					mistakes[0] = 0;
				else				// player B
					mistakes[1] = 0;
			}

			markedBoard[i][posJ] = 'X'; // marking the curr tile
			countTiles++;				// counting fitted tiles
		}
		else break;
	} // end of for
	if (countTiles == shipSize)
	{
		if (checkValid)
		{
			if (possibleAdj == 1)
				mistakes[6] = 1;
			return true;
		}
	}
	// end of vertical check

	// shape isn't horizental or vertical
	if (possibleAdj == 1)
		mistakes[6] = 1;
	if (player == 0)	// player A
	{
		if (mistakes[0] == 0)
			mistakes[0] = shipType;
	}
	else			// player B
		if (mistakes[1] == 0)
			mistakes[1] = shipType;
	return false;
}

bool checkBound(char** board, char shipType, int i, int j, int* mistakes, int player, int* possibleAdj)
{
	char checkChar = board[i][j];
	if (checkChar != ' ')	// if the tile isn't sea
	{
		if (checkChar == shipType)	// case of adjacent with same ship of the same player
		{
			// mark appropriate mistake - invalid shape. no need to count as valid ship of the player.
			if (player == 0)	// player A
			{	
				if (mistakes[0] == 0)
					mistakes[0] = checkChar;
			}
			else			// player B
				if (mistakes[1] == 0)
					mistakes[1] = checkChar;
			return true;
		}
		else								// case of adjacent with another ship of the same/another player
		{
			// mark appropriate mistake - adjacent ships.
			*possibleAdj = 1;
			return false;
		}
	}
	return false;
}

void printBoard(char** board)
{
	cout << "===========SHOWING BOARD===========";
	for (int i = 0; i < ROWS_WIDTH; i++)
	{
		cout << " " << endl;
		for (int j = 0; j < COLS_WIDTH; j++)
		{
			cout << board[i][j] << " ";
		}
	}
	cout << " " << endl;
	cout << "===================================";
	cout << " " << endl;
}