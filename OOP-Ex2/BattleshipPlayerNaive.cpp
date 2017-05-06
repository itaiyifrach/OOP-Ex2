#include "BattleshipPlayerNaive.h"


void BattleshipPlayerNaive::setBoard(int player, const char** playerBoard, int numRows, int numCols) {
	playerNum = player;
	cols = numCols;
	rows = numRows;
	this->myBoard = new char*[numRows];
	for (int i = 0; i < numRows; ++i)
	{
		this->myBoard[i] = new char[numCols];
		for (int j = 0; j < numCols; ++j)
		{
			if (playerBoard[i][j] != 32)
			{
				this->myBoard[i][j] = '%';
				if (i > 0)
				{
					this->myBoard[i - 1][j] = '%';
				}
				if (i < numRows)
				{
					this->myBoard[i + 1][j] = '%';
				}
				if (j > 0)
				{
					this->myBoard[i][j - 1] = '%';
				}
				if (j < numCols)
				{
					this->myBoard[i][j + 1] = '%';
				}
			}
			else
				this->myBoard[i][j] = playerBoard[i][j];
		}
	}
}


std::pair<int, int> BattleshipPlayerNaive::attack() {
	std::pair<int, int> result{ currentAttack.first - 1, currentAttack.second - 1 };
	
	while (myBoard[result.first][result.second] == '%')
	{
		if (result.second < cols)
		{
			result.second++;
		}
		else
		{
			if (result.first < rows)
			{
				result.first++;
			}
			else //not possible that we try attacking in every cell of the board and even get here.
			{
				result.first = -1;
				result.second = -1;
				return result;
			}
			result.second = 0;
		}
	}
	
	result.first++;
	result.second++;
	currentAttack = result;
	return currentAttack;
}

IBattleshipGameAlgo* GetAlgorithm()
{
	return new BattleshipPlayerNaive();

}

