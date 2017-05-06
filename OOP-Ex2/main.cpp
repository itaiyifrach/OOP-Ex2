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

	if (initialize(argc, argv, mainBoard, ROWS, COLS, basePath, &useAnimation, &delay) < 0) {
		delete[](mainBoard);
		return -1;
	}
	//creates a vector of all *.dll files in the given path
	vector<string> fileNames=getDLLNames(basePath);
	if(fileNames.size()<2)
	{
		delete[](mainBoard);
		return -1;
	}
	//loads first DLL and creates playerA
	IBattleshipGameAlgo* playerA = loadAlgo(basePath, fileNames[0]);
	if(!playerA)
	{
		delete[](mainBoard);
		return -1;
	}
	//init for playerA(relevant only if playerA type is BattelshipPlayerFromFile
	if (!playerA->init(basePath))
	{
		cout << INIT_FAILED << basePath << "//" << fileNames[0] << endl;
		return -1;
	}
	
	//loads second DLL and creates playerB
	IBattleshipGameAlgo* playerB = loadAlgo(basePath, fileNames[1]);
	if (!playerB)
	{
		delete[](mainBoard);
		return -1;
	}
	//init for playerB(relevant only if playerB type is BattelshipPlayerFromFile
	if (!playerB->init(basePath))
	{
		cout << INIT_FAILED << basePath << "//" << fileNames[1] << endl;
		return -1;
	}

	BattleshipGame game(playerA, playerB, mainBoard );
	if (game.isCorrupted())
	{
		delete[](mainBoard);
		return -1;
	}
	game.playGame();

	delete[](mainBoard);
	return 0;
}