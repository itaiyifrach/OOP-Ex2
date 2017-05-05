#pragma once

class BoardUtils
{
public:
	//check if ship is horizontal or vertical
	static bool isVertical(char** board, int rows, int cols, int i, int j);

	static void markSinkedShip(char** board, int rows, int cols, int i, int j, char mark);

	//returns the score-for-a-sink of a ship which is located in (i,j)th coordinate on mainBoard
	static int getScoreForSector(char boardSymbol);

	static bool selfHit(char boardSymbol, int attackPlayer);
};