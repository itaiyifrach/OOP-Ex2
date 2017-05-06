#include "initialize.h"
#include "BattleshipGame.h"

int main(int argc, char** argv) 
{
	string basePath;
	// by default: use animation with delay of 1.5s
	bool useAnimation = true;
	int delay = 1500;

	char** mainBoard = new char*[ROWS];
	for (int i = 0; i < ROWS; ++i)
		mainBoard[i] = new char[COLS];
		
	if (initialize(argc, argv, mainBoard, ROWS, COLS,basePath, &useAnimation, &delay) < 0) {
		delete[](mainBoard);
		return -1;
	}

	
	//something with dlls
	string MATANSPATH;

	//decide what players we construct.
	//if we decide playerFromFile:
	BattleshipPlayerFromFile playerA;
	playerA.setBoard(0, (const char**)mainBoard, ROWS, COLS);
	if (!playerA.init(basePath))
	{
		//TODO: add dll path to prints
		cout << INIT_FAILED << endl;
		return -1;
	}
			
	BattleshipPlayerFromFile playerB;
	playerB.setBoard(1, (const char**)mainBoard, ROWS, COLS);
	if(!playerB.init(basePath))
	{
		//TODO: add dll path to prints
		cout << INIT_FAILED << endl;
		return -1;
	}
	
	BattleshipGame game(&playerA, &playerB, mainBoard, MATANSPATH);
	if (game.isCorrupted())
	{
		delete[](mainBoard);
		return -1;
	}
	game.playGame(useAnimation, delay);

	delete[](mainBoard);
	return 0;
}