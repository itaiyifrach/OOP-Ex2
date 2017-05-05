#include "initialize.h"
#include "BattleshipGame.h"

int main(int argc, char** argv) {
	string attackAPath, attackBPath;
	char** mainBoard = new char*[ROWS];
	for (int i = 0; i < ROWS; ++i)
		mainBoard[i] = new char[COLS];
		
	if (initialize(argc, argv, mainBoard, ROWS, COLS, attackAPath, attackBPath) < 0) {
		delete[](mainBoard);
		return -1;
	}

	//something with dlls
	string MATANSPATH;

	//decide what players we construct.
	//if we decide playerFromFile:
	BattleshipPlayerFromFile PlayerA;
	BattleshipPlayerFromFile PlayerB;
	
	BattleshipGame game(&PlayerA, &PlayerB, mainBoard, MATANSPATH);
	if (game.isCorrupted())
	{
		delete[](mainBoard);
		return -1;
	}
	game.playGame();

	delete[](mainBoard);
	return 0;
}