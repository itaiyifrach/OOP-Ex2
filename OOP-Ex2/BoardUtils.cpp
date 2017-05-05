#include "BoardUtils.h"

bool BoardUtils::isVertical(char** board, int rows, int cols, int i, int j) {
	if (((j == 0) && ((board[i][1] == 32) || (board[i][1] == '%'))) || ((j == (cols - 1)) && ((board[i][cols - 2] == 32) || (board[i][cols - 2] == '%'))))
		return true;
	if ((j == 0) || (j == (cols - 1)))
		return false;
	if (((board[i][j - 1] == 32) || (board[i][j - 1] == '%')) && ((board[i][j + 1] == 32) || (board[i][j + 1] == '%')))
		return true;
	return false;
}

void BoardUtils::markSinkedShip(char** board, int rows, int cols, int i, int j, char mark){
	if (BoardUtils::isVertical(board, rows, cols, i, j)) {
		//find upper corner
		while ((i > 0) && (board[i][j] != 32) && (board[i][j] != '%'))
			i--;
		if ((board[i][j] == 32) || (board[i][j] == '%'))
			i++;
		while ((i < rows) && (board[i][j] != 32) && (board[i][j] != '%'))
			board[i++][j] = mark;
	}
	else {
		//find left corner
		while ((j > 0) && (board[i][j] != 32) && (board[i][j] != '%'))
			j--;
		if ((board[i][j] == 32) || (board[i][j] == '%'))
			j++;
		while ((j < cols) && (board[i][j] != 32) && (board[i][j] != '%'))
				board[i][j++] = mark;
	}
}

int BoardUtils::getScoreForSector(char boardSymbol) {
	switch (boardSymbol) {
	case 'b':
	case 'B':
		return 2;
	case 'p':
	case 'P':
		return 3;
	case 'm':
	case 'M':
		return 7;
	case 'd':
	case 'D':
		return 8;
	default:
		return 0;
	}
}

bool BoardUtils::selfHit(char boardSymbol, int attackPlayer)
{
	if (attackPlayer == 0)
	{
		switch (boardSymbol) {
		case 'B':
		case 'P':
		case 'M':
		case 'D':
			return true;
		default:
			return false;
		}
	}
	switch (boardSymbol) {
	case 'b':
	case 'p':
	case 'm':
	case 'd':
		return true;
	default:
		return false;
	}
}