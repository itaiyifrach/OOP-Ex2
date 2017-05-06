#include "initialize.h"

int initialize(int argc, char** argv, char** board, int numRows, int numCols, string& basePath, bool* useAnimation, int* delay) {
	string boardPath;
	//if didn't get argument- path is the cwd, else it is the path given in argv[1]
	auto temp = _fullpath(nullptr, "", 256);
	basePath = temp;
	free(temp);

	if (argc == 2)
	{
		if (argv[1] == "-quiet")
		{
			*useAnimation = false;
		}
		else
		{
			char* temp = _fullpath(nullptr, argv[1], 256);
			basePath = temp;
			free(temp);
		}
	}
	else if (argc == 3)
	{
		if (argv[1] == "-delay")
		{
			*delay = atoi(argv[2]);
		}
		else
		{
			char* temp = _fullpath(nullptr, argv[1], 256);
			basePath = temp;
			free(temp);
			*useAnimation = false;
		}
	}
	else if (argc == 4)
	{
		char* temp = _fullpath(nullptr, argv[1], 256);
		basePath = temp;
		free(temp);
		*delay = atoi(argv[3]);
	}
	
	//check if there was no board file
	if(parsePath(basePath,boardPath)!=0)
	{		
		return -1;
	}

	// \n and \r\n compatible.
	// read getline(inf) X 10, getc from line X 10. empty line / invalid char is a space.
	parseBoard(boardPath, board,numRows,numCols);

	// check if the board is valid
	int mistakes[7] = { 0 };
	char shipMistakeTypeA = 0, shipMistakeTypeB = 0;
	if (checkBoard(board, mistakes) == false)
	{
		for (auto i = 0; i < 7; i++)
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



int parsePath(const string& basePath,string& file_path)
{
	HANDLE dir;
	WIN32_FIND_DATAA fileData; //data struct for file
							 
	string file_suffix = BOARD_SUFFIX; //only *.sboard files
	dir = FindFirstFileA((basePath + file_suffix).c_str(), &fileData); // Notice: Unicode compatible version of FindFirstFile
	if (dir != INVALID_HANDLE_VALUE) //check if the dir opened successfully
	{
		string fileName = fileData.cFileName;
		file_path = basePath + "\\" + fileName;		
	}
	else
	{
		//check if no .sboard file was found or there was a problem with the dir,and print error accordingly
		if (GetLastError() != ERROR_FILE_NOT_FOUND)
		{
			cout << WRONG_PATH << basePath << endl;
		}
		else
		{
			cout << MISSING_BOARD << basePath << endl;
		}
		return -1;
	}
			
	return 0;
}

void parseBoard(const string& boardPath, char**& board, int rows, int cols)
{
	ifstream ifs(boardPath);
	string line;
	for (auto i = 0; i < rows; i++)
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
		for(j;j<cols;j++)
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

vector<string> getLibraries(string& path)
{
	HANDLE dir;
	WIN32_FIND_DATAA fileData; //data struct for file
	vector<string> fileNames;
	
	// iterate over *.dll files in path	

	string file_suffix = DLL_SUFFIX; // only .dll endings
	dir = FindFirstFileA((path + file_suffix).c_str(), &fileData); // Notice: Unicode compatible version of FindFirstFile
	if (dir != INVALID_HANDLE_VALUE) //check if the dir opened successfully
	{
		// test each file suffix and set variables as needed
		do
		{
			string fileName = fileData.cFileName;			
			cout << fileName << endl;			
			fileNames.push_back(fileName);
			
		} while (FindNextFileA(dir, &fileData)); // Notice: Unicode compatible version of FindNextFile
	}
	else
	{
		cout << MISSING_ALGO << path << endl;
		return fileNames;
	}
	if(fileNames.size()<2)
	{
		cout << MISSING_ALGO << path << endl;
		return fileNames;
	}
	sort(fileNames.begin(), fileNames.end());
	return fileNames;

}

IBattleshipGameAlgo* loadAlgo( const string& path, const string& fileName)
{
	// define function of the type we expect
	typedef IBattleshipGameAlgo *(*GetAlgoFuncType)();
	GetAlgoFuncType getAlgoFunc;
	string algoPath = path + "//" + fileName;
	// Load dynamic library
	HINSTANCE hDll = LoadLibraryA(algoPath.c_str()); // Notice: Unicode compatible version of LoadLibrary
	if (!hDll)
	{
		cout << LOAD_LIB_ERR << endl;
		return nullptr;
	}

	// Get function pointer
	getAlgoFunc = (GetAlgoFuncType)GetProcAddress(hDll, "GetAlgo");
	if (!getAlgoFunc)
	{
		cout << LOAD_FUNC_ERR << endl;
		return nullptr;
	}
	return getAlgoFunc();

}

