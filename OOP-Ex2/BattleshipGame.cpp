#include "BattleshipGame.h"

void BattleshipGame::initPlayerBoard(int playerNum) const{
	char** playerBoard = new char*[ROWS];
	for (int i = 0; i < rows; ++i)
	{
		playerBoard[i] = new char[COLS];
		for (int j = 0; j < cols; j++) {
			if (playerNum == 0) {
				if ((65 < mainBoard[i][j]) && (mainBoard[i][j] < 90))
					playerBoard[i][j] = mainBoard[i][j];
				else
					playerBoard[i][j] = 32;
			}
			else {
				if ((97 < mainBoard[i][j]) && (mainBoard[i][j] < 122))
					playerBoard[i][j] = mainBoard[i][j];
				else
					playerBoard[i][j] = 32;
			}
		}
	}
	if (playerNum == 1)
	{
		PlayerB->setBoard(playerNum, (const char**) playerBoard, rows, cols);
	} 
	else
	{
		PlayerA->setBoard(playerNum, (const char**) playerBoard, rows, cols);
	}
	delete[] playerBoard;
}

void BattleshipGame::playGame() {
	pair<AttackResult, bool> AttackResTupple;
	AttackResult currAttackRes;
	int i, j, sectorScore;
	bool selfHit = false;
	bool wasAlreadyHit;
	bool endGame = false;

	printColorBoard();

	//0 iff its A's turn
	int turnOf = 0;

	//initialize first attack (check if A/B have moves)
	pair<int, int> currAttack = PlayerA->attack();
	if (currAttack.first == -1)	
	{
		currAttack = PlayerB->attack();
		if (currAttack.first == -1)	
		{
			endGame = true;
		} 
		else 
		{
			turnOf = 1;
		}
	}

	while ((!endGame) && (numOfShipsA) && (numOfShipsB)) {
		//attacks are saved between 1..ROWS in the files. We shift the coordinates to 0..(ROWS-1)
		i = currAttack.first - 1;
		j = currAttack.second - 1;
		
		if (BoardUtils::selfHit(mainBoard[i][j], turnOf)) {
			selfHit = true;
		}

		//check for the sink-score of the ship in (i,j)th sector
		sectorScore = BoardUtils::getScoreForSector(mainBoard[i][j]);

		//check for the attack result and update the mainBoard according to the result
		AttackResTupple = getAttackResult(i, j);
		currAttackRes = AttackResTupple.first;
		wasAlreadyHit = AttackResTupple.second;
		
		//check for sinks to update the score
		if (currAttackRes == AttackResult::Sink)
		{
			if (((!selfHit) && (turnOf == 0)) || ((selfHit) && (turnOf == 1)))
			{
				scoreA += sectorScore;
				numOfShipsB--;
			}
			else
			{
				scoreB += sectorScore;
				numOfShipsA--;
			}
		}			
		
		//Notify players on results
		PlayerA->notifyOnAttackResult(turnOf, i, j, currAttackRes);
		PlayerB->notifyOnAttackResult(turnOf, i, j, currAttackRes);

		//Switch if current player missed / made a self-hit
		if ((currAttackRes == AttackResult::Miss) 
			|| ((currAttackRes == AttackResult::Hit) && (wasAlreadyHit))
			|| ((currAttackRes == AttackResult::Hit) && selfHit && (!wasAlreadyHit)))
		{
			if (turnOf == 0)
			{
				turnOf = 1;
			}
			else
			{
				turnOf = 0;
			}
		}
		getNextAttack(turnOf, endGame, currAttack);
		selfHit = false;
	}

	if ((numOfShipsA) && (!numOfShipsB))
		cout << PLAYER_A_WON_STR << endl;
	else if ((!numOfShipsA) && (numOfShipsB))
		cout << PLAYER_B_WON_STR << endl;
	cout << POINTS_STR << endl;
	cout << PLAYER_A_POINTS_STR << scoreA << endl;
	cout << PLAYER_B_POINTS_STR << scoreB << endl;
}

void BattleshipGame::getNextAttack(int& turnOf, bool& endGame, pair<int, int>& currAttack) const
{
	if (turnOf == 0)
	{
		currAttack = PlayerA->attack();
		if (currAttack.first == -1)
		{
			currAttack = PlayerB->attack();
			if (currAttack.first == -1)
			{
				endGame = true;
			} else {
				turnOf = 1;
			}
		}
	}
	else
	{
		currAttack = PlayerB->attack();
		if (currAttack.first == -1)
		{
			currAttack = PlayerA->attack();
			if (currAttack.first == -1)
			{
				endGame = true;
			}
			else
			{
				turnOf = 0;
			}
		}
	}
}


std::pair<AttackResult, bool> BattleshipGame::getAttackResult(int i, int j) const{
	std::pair<AttackResult, bool> result;
	if (mainBoard[i][j] == 32) {
		result.first = AttackResult::Miss;
		result.second = false;
	}
	else if (mainBoard[i][j] == 'X') {
		result.first = AttackResult::Hit;
		result.second = true;
	}
	else if (updateBaordAndCheckSink(i, j)) {
		result.first = AttackResult::Sink;
		result.second = false;
	}
	else {
		result.first = AttackResult::Hit;
		result.second = false;
	}
	return result;
}

bool BattleshipGame::updateBaordAndCheckSink(int i, int j) const {
	if (BoardUtils::isVertical(mainBoard, rows, cols, i, j)) {
		int rowIndex = i;
		//find upper corner and save it in "upper"
		while ((rowIndex > 0) && (mainBoard[rowIndex][j] != 32))
			rowIndex--;	
		if (mainBoard[rowIndex][j] == 32)
			rowIndex++;
		int upper = rowIndex;

		//count how many parts of the ship remain un-hit
		int count = 0;
		while ((rowIndex < rows) && (mainBoard[rowIndex][j] != 32)) {
			if (mainBoard[rowIndex][j] != 'X')
				count++;
			rowIndex++;
		}

		//if we have only one un-hit part and it was the initial part in (i,j), we erase the ship and return that it's a sink
		if ((count == 1) && (mainBoard[i][j]!='X') && (mainBoard[i][j] != 32)) {
			rowIndex = upper;
			while ((rowIndex < rows) && (mainBoard[rowIndex][j] != 32))
				mainBoard[rowIndex++][j] = 32;
			return true;
		}
		//if it's not a sink we just update the (i,j) coordinate on the mainBoard as a hit and return it isn't a sink
		mainBoard[i][j] = 'X';
		return false;
	}
	else {
		int colIndex = j;
		//find left corner and save it in "left"
		while ((colIndex > 0) && (mainBoard[i][colIndex] != 32))
			colIndex--;
		if (mainBoard[i][colIndex] == 32)
			colIndex++;
		int left = colIndex;

		//count how many parts of the ship remain un-hit
		int count = 0;
		while ((colIndex < cols) && (mainBoard[i][colIndex] != 32))
			if (mainBoard[i][colIndex++] != 'X')
				count++;

		//if we have only one un-hit part, it was the initial part in (i,j), we erase the ship and return that it's a sink
		if (count == 1) {
			colIndex = left;
			while ((colIndex < cols) && (mainBoard[i][colIndex] != 32))
				mainBoard[i][colIndex++] = 32;
			return true;
		}
		//if it's not a sink we just update the (i,j) coordinate on the mainBoard as a hit and return it isn't a sink
		mainBoard[i][j] = 'X';
		return false;
	}
}

void BattleshipGame::printColorBoard()
{
	// 9 = BLUE, 12 = RED
	int playerAColor = 9, playerBColor = 12;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	/*
	for (int k = 1; k < 255; k++)
	{
		SetConsoleTextAttribute(hConsole, k);
		cout << k << " I want to be nice today!" << endl;
	}
	*/

	cout << "=============BATTLESHIP=============";
	for (int i = 0; i < rows; ++i)
	{
		cout << " " << endl;
		for (int j = 0; j < cols; j++) {
			if ((65 < mainBoard[i][j]) && (mainBoard[i][j] < 90)) {				// player A
				cout << playerAColor << mainBoard[i][j] << " ";
			}
			else if ((97 < mainBoard[i][j]) && (mainBoard[i][j] < 122))  {		// player B
				cout << playerBColor << mainBoard[i][j] << " ";
			}
		}
	}
	cout << " " << endl;
	cout << "====================================";
	cout << " " << endl;
}

void BattleshipGame::gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}